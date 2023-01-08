#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>
#include "convexhull.h"
#include "generation.h"
#include "graphics.h"

/**
 * @brief Calcule l'orientation du triangle
 * @param a Adresse du point A
 * @param b Adresse du point B
 * @param c Adresse du point C
 * @return L'orientation, en particulier est positif
 * si le triangle est direct, négatif sinon 
 */
inline double CVH_direction_triangle(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

/**
 * @brief Ajoute un point au polygone convexe.
 * Les points sont ajoutés inconditionnellement si le polygône
 * possède moins de 2 sommets, et ajoute le troisième positivement
 * de la la même manière que les sommets suivants.
 * Les sommets qui n'appartiennent plus à l'ensemble seront déplacés
 * vers la liste reste.
 * Si le point ne fait pas parti du polygône, il ne sera PAS
 * automatiquement ajouté à la liste reste.
 * @param point Adresse du point à ajouter.
 * @param convex Adresse de l'objet ConvexHull auquel sera ajouté le point.
 * @param reste Adresse de la liste des points n'appartenant plus au polygône.
 * @return 1 si le point a été ajouté à l'enveloppe convexe, 0 sinon.
 */
int CVH_add(Point* point, ConvexHull* convex, ListPoint* reste) {
    Vertex* new_entry;
    Polygone* poly = &(convex->poly);
    Vertex *vtx, *vtx1;

    if (convex->current_len < 2) {
        new_entry = CVH_new_vertex_pointer(point);
        CIRCLEQ_INSERT_TAIL(poly, new_entry, entries);
        convex->current_len++;
        convex->max_len++;
        return 1;
    }

    CIRCLEQ_FOREACH(vtx, poly, entries) {
        vtx1 = CIRCLEQ_LOOP_NEXT(poly, vtx, entries);

        if (IS_DIRECT_TRIANGLE(*point, *vtx->p, *vtx1->p))
            continue;

        new_entry = CVH_new_vertex_pointer(point);

        CIRCLEQ_INSERT_AFTER(poly, vtx, new_entry, entries);
        CIRCLEQ_SET_AS_FIRST(poly, new_entry, entries);
        convex->current_len++;

        CVH_cleaning(convex, reste);
        // printf("Nombre de points supprimés: %d\n", n_deleted);

        return 1;
    }
    return 0;
}

/**
 * @brief Ajoute un point à une des enveloppes convexe de la liste.
 * 
 * @param convexs Adresse de l'entête de la liste d'enveloppes convexes.
 * @param convex Elément de la liste à partir duquel on commence l'algorithme
 * Par défaut veuillez choisir le premier élement de la liste.
 * @param point Adresse d'un point à ajouter.
 */
void CVH_add_inception_recursif(ListConvexHull* convexs, ConvexHullEntry* convex, Point* point) {
    
    if (CIRCLEQ_EMPTY(convexs) || (void*) convexs == (void*) convex) {
        ConvexHullEntry* new_entry = CVH_new_convexhullentry(point);
        CIRCLEQ_INSERT_TAIL(convexs, new_entry, entries);
        return;
    }

    ListPoint reste_interieur;
    CIRCLEQ_INIT(&reste_interieur);
    bool ajoute = CVH_add(point, convex->convex, &reste_interieur);

    if (!ajoute) {
        CVH_add_inception_recursif(convexs, CIRCLEQ_NEXT(convex, entries), point);
    }

    Vertex* vtx;
    CIRCLEQ_FOREACH(vtx, &reste_interieur, entries) {
        CVH_add_inception_recursif(convexs, CIRCLEQ_NEXT(convex, entries), vtx->p);
    }
    CVH_free_vertex_list(&reste_interieur, false);

    return;
}

/**
 * @brief Fonction auxillière de CVH_add().
 * Supprime les points ne faisant plus partis du polygône
 * convexe.
 * Le polygône doit commencer à partir du dernier point ajouté.
 * @param convex Adresse de l'objet ConvexHull
 * @param reste Adresse de la liste où déplacer les points sortis
 * de l'ensemble.
 * @return Nombre de points supprimés.
 */
int CVH_cleaning(ConvexHull* convex, ListPoint* reste) {
    int deleted_points = 0;
    Vertex *vtx, *vtx1, *vtx2;
    Polygone* poly = &(convex->poly);

    #ifdef DEBUG_CVH_CLEANING
    fprintf(stderr, "Itération avant :\n");
    #endif

    for (vtx = CIRCLEQ_FIRST(poly); ;) {
        vtx1 = CIRCLEQ_LOOP_NEXT(poly, vtx, entries);
        vtx2 = CIRCLEQ_LOOP_NEXT(poly, vtx1, entries);

        #ifdef DEBUG_CVH_CLEANING
        GFX_draw_debug_triangle_direction(convex, *vtx->p, *vtx1->p, *vtx2->p);
        #endif

        if (!IS_DIRECT_TRIANGLE(*vtx->p, *vtx1->p, *vtx2->p)) {
            CIRCLEQ_REMOVE(poly, vtx1, entries);
            CIRCLEQ_INSERT_TAIL(reste, vtx1, entries);
            deleted_points++;
        }
        else {
            //CIRCLEQ_SET_AS_FIRST(poly, vtx, entries);
            break;
        }
    }

    #ifdef DEBUG_CVH_CLEANING
    fprintf(stderr, "Itération arrière :\n");
    #endif

    for (; ;) {
        vtx1 = CIRCLEQ_LOOP_PREV(poly, vtx, entries);
        vtx2 = CIRCLEQ_LOOP_PREV(poly, vtx1, entries);

        #ifdef DEBUG_CVH_CLEANING
        GFX_draw_debug_triangle_direction(convex, *vtx->p, *vtx2->p, *vtx1->p);
        #endif

        if (!IS_DIRECT_TRIANGLE(*vtx->p, *vtx2->p, *vtx1->p)) {
            CIRCLEQ_REMOVE(poly, vtx1, entries);
            CIRCLEQ_INSERT_TAIL(reste, vtx1, entries);
            deleted_points++;
        }
        else
            break;
    }
    convex->current_len -= deleted_points;

    if (convex->current_len > convex->max_len)
        convex->max_len = convex->current_len;

    return deleted_points;
}

/**
 * @brief Crée un polygône convexe avec la liste
 * de points fournie.
 * 
 * @param points Liste des points.
 * @param convex Adresse de l'objet ConvexHull de destination.
 * @param reste Adresse de la liste des points auquel seront
 * ajoutés les points n'appartenant pas au polygône convexe.
 * @param callback Fonction à appeler après chaque ajout de point,
 * prenant en paramètre l'objet ConvexHull et l'adresse de
 * la liste de points ``reste``.
 */
void CVH_points_to_ConvexHull(
    ListPoint* points,
    ConvexHull* convex, ListPoint* reste,
    void (*callback)(ConvexHull*, ListPoint*)
) {
    Vertex* vtx;
    Vertex* new_entry;
    CIRCLEQ_INIT(reste);

    CIRCLEQ_FOREACH(vtx, points, entries) {
        short added = CVH_add(vtx->p, convex, reste);
        if (!added) {
            new_entry = CVH_new_vertex_pointer(vtx->p);
            CIRCLEQ_INSERT_HEAD(reste, new_entry, entries);
        }
        if (callback)
            callback(convex, reste);
    }
}

/**
 * @brief Crée une liste de polygône convexes imbriqués, à partir
 * de la liste de points fournies 
 * 
 * @param points Adresse de la liste de points.
 * @param convexs Adresse de l'entête de la liste de ConvexHull
 * de destination
 * @param callback Fonction à appeler après chaque ajout de point,
 * prenant en paramètre l'objet ListConvexHull et l'adresse de
 */
void CVH_points_to_ListConvexHull(
    ListPoint* points,
    ListConvexHull* convexs,
    void (*callback)(ListConvexHull*)
) {
    Vertex* vtx;

    CIRCLEQ_FOREACH(vtx, points, entries) {
        CVH_add_inception_recursif(convexs, convexs->cqh_first, vtx->p);
        if (callback)
            callback(convexs);
    }
}

/**
 * @brief Crée une liste de polygones convexes imbriqués.
 * Cette algorithme est itératif, mais ne permet de créer
 * une liste d'enveloppes uniquement avec une liste de points
 * déjà créés, et ne permet d'ajouter qu'un seul point.
 * En effet, nous itérons succesivement sur les points
 * intérieurs.
 * 
 * @param convexs Adresse de l'entête de la liste de
 * polygones convexes de destination.
 * @param points Adresse de la liste des points.
 * @return int Taille de la liste
 */
int CVH_add_inception_iteratif(ListConvexHull* convexs, ListPoint* points) {
    ListPoint reste, points2;
    ConvexHullEntry* convex_entry;
    ConvexHull* convex;
    CIRCLEQ_INIT(convexs);
    CIRCLEQ_INIT(&reste);
    CIRCLEQ_INIT(&points2);

    int i = 0;
    do {
        convex = CVH_init_convexhull();
        convex->color = GFX_map_color(i);
        convex_entry = malloc(sizeof(ConvexHullEntry));
        convex_entry->convex = convex;
        CVH_points_to_ConvexHull(i == 0 ? points : &points2, convex, &reste, NULL);
        CIRCLEQ_INSERT_TAIL(convexs, convex_entry, entries);
        CIRCLEQ_MOVE_TO(&reste, &points2, entries);
        ++i;
    } while (!CIRCLEQ_EMPTY(&reste));

    return i;
}

/**
 * @brief Ajoute un point à une liste de points.
 * 
 * @param points Adresse de la liste auquel sera ajouté le point.
 * @param point Le point à ajouter.
 * @return Adresse du point alloué.
 */
Point* CVH_add_user_point(ListPoint* points, Point point) {
    Vertex* new_entry;
    new_entry = CVH_new_vertex((Point) {point.x, point.y});
    CIRCLEQ_INSERT_TAIL(points, new_entry, entries);

    return new_entry->p;
}

/**
 * @brief Ajoute un point à un polygône convexe si il y appartient,
 * sinon l'ajoute à la liste reste.
 * @param convex Adresse de l'objet ConvexHull.
 * @param point Adresse du point alloué.
 * @param reste Adresse de la liste des points n'appartenant pas au polygône.
 * @return int 
 */
int CVH_add_to_convex(ConvexHull* convex, Point* point, ListPoint* reste) {
    if (convex->current_len == 1) {
        point->x += 1e-12;
        point->y += 1e-12;
    }
    int added = CVH_add(point, convex, reste);
    if (!added) {
        Vertex* new_entry = CVH_new_vertex_pointer(point);
        CIRCLEQ_INSERT_TAIL(reste, new_entry, entries);
    }
    // printf("Vertex(%d, %d)\n", new_entry->p->x, new_entry->p->y);
    return added;
}

/**
 * @brief Alloue un Vertex et un point en mémoire.
 * 
 * @param point Point à allouer.
 * @return Adresse du Vertex créé, NULL en cas d'erreur.
 */
Vertex* CVH_new_vertex(Point point) {
    Point* new_point = malloc(sizeof(Point));
    Vertex* new_vtx = malloc(sizeof(Vertex));
    
    if (!new_point || !new_vtx)
        return NULL;

    *new_point = point;
    new_vtx->p = new_point;
    
    return new_vtx;
}

/**
 * @brief Alloue un Vertex en mémoire.
 * 
 * @param point Adresse du point à affecter au vertex.
 * @return Adresse du Vertex créé, NULL en cas d'erreur.
 */
Vertex* CVH_new_vertex_pointer(Point* point) {
    Vertex* new_vtx = malloc(sizeof(Vertex));
    
    if (!new_vtx)
        return NULL;

    new_vtx->p = point;

    return new_vtx;
}

/**
 * @brief Initialise un objet ConvexHull
 * @return Adresse de l'instance du polygône convexe, ou
 * NULL en cas d'échec.
 */
ConvexHull* CVH_init_convexhull(void) {
    ConvexHull* convex = calloc(1, sizeof(ConvexHull));
    if (!convex)
        return NULL;

    CIRCLEQ_INIT(&convex->poly);
    convex->color = MLV_rgba(255, 0, 0, 50);
    
    return convex;
}

/**
 * @brief Alloue une entrée de liste de ConvexHull,
 * et lui alloue également un ConvexHull avec l'adresse
 * du point.
 * La couleur du polygône change successivement
 * à chaque appel, selon le mapping de la fonction
 * GFX_map_color().
 * 
 * @param p Adresse du point à ajouter à l'enveloppe
 * @return ConvexHullEntry* Adresse de l'entrée de liste de ConvexHull, ou
 * NULL en cas d'échec lors d'une allocation.
 */
ConvexHullEntry* CVH_new_convexhullentry(Point* p) {
    ConvexHullEntry* new_entry;
    ConvexHull* new_convex;
    Vertex* new_vtx;
    static int i = 0;

    new_entry = malloc(sizeof(ConvexHullEntry));
    new_convex = CVH_init_convexhull();
    new_vtx = CVH_new_vertex_pointer(p);
    
    if (!new_entry || !new_convex || !new_vtx) {
        return NULL;
    }

    CIRCLEQ_INSERT_TAIL(&new_convex->poly, new_vtx, entries);
    new_convex->current_len = 1;
    new_convex->color = GFX_map_color(i);
    new_entry->convex = new_convex;
    
    i++;
    
    return new_entry;
}

/**
 * @brief Libére la mémoire allouée pour une liste de points.
 * 
 * @param lst Adresse de la liste
 * @param free_points Spécifie si on libère également
 * les points alloués.
 */
void CVH_free_vertex_list(ListPoint* lst, bool free_points) {
    Vertex *vtx = CIRCLEQ_FIRST(lst), *vtx2;

    while (vtx != (void*) lst) {
        vtx2 = CIRCLEQ_NEXT(vtx, entries);
        if (free_points)
            free(vtx->p);
        free(vtx);
        vtx = vtx2;
    }
    CIRCLEQ_INIT(lst);
}

/**
 * @brief Libère la mémoire allouée pour un
 * objet ConvexHull, sans libérer les points.
 * 
 * @param convex Adresse de l'instance ConvexHull
 */
void CVH_free_convexhull(ConvexHull* convex) {
    CVH_free_vertex_list((ListPoint*) &convex->poly, 0);
    free(convex);
}

void CVH_free_ListConvexHull(ListConvexHull* convexs) {
    ConvexHullEntry *entry = CIRCLEQ_FIRST(convexs), *entry2;
    
    while (entry != (void*) convexs) {
        entry2 = CIRCLEQ_NEXT(entry, entries);
        CVH_free_convexhull(entry->convex);
        free(entry);
        entry = entry2;
    }
    CIRCLEQ_INIT(convexs);
}
