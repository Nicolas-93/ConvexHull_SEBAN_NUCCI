#include <stdlib.h>
#include <stdio.h>
#include "generation.h"
#include "convexhull.h"
#include "MLV/MLV_all.h"
#ifdef DEBUG_CVH_CLEANING
#include "graphics.h"
#endif

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
        new_entry = GEN_new_vertex_pointer(point);
        CIRCLEQ_INSERT_TAIL(poly, new_entry, entries);
        convex->current_len++;
        convex->max_len++;
        return 1;
    }

    CIRCLEQ_FOREACH(vtx, poly, entries) {
        vtx1 = CIRCLEQ_TRUE_NEXT(poly, vtx);

        if (IS_DIRECT_TRIANGLE(*point, *vtx->p, *vtx1->p))
            continue;

        new_entry = GEN_new_vertex_pointer(point);

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
        vtx1 = CIRCLEQ_TRUE_NEXT(poly, vtx);
        vtx2 = CIRCLEQ_TRUE_NEXT(poly, vtx1);

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
        vtx1 = CIRCLEQ_TRUE_PREV(poly, vtx);
        vtx2 = CIRCLEQ_TRUE_PREV(poly, vtx1);

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
 * @param points Liste des points.
 * @param convex Adresse de l'objet ConvexHull initialisé.
 * @param reste Adresse de la liste des points auquel seront
 * ajoutés les points n'appartenant pas au polygône convexe.
 * @param callback Fonction à appeler après chaque ajout de point,
 * prenant en paramètre l'objet ConvexHull et l'adresse de
 * la liste de points ``reste``.
 */
void CVH_points_to_convex(
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
            new_entry = GEN_new_vertex_pointer(vtx->p);
            CIRCLEQ_INSERT_HEAD(reste, new_entry, entries);
        }
        if (callback)
            callback(convex, reste);
    }
}

/**
 * @brief Crée une liste de polygones convexes imbriqués.
 * 
 * @param points Adresse de la liste des points.
 * @param convexs Adresse de la liste de polygones convexes
 * de destination.
 */
void CVH_convexhull_inception(ListPoint* points, ConvexHulls* convexs) {
    ListPoint reste, points2;
    ConvexHullEntry* convex_entry;
    ConvexHull* convex;
    CIRCLEQ_INIT(convexs);
    CIRCLEQ_INIT(&reste);
    CIRCLEQ_INIT(&points2);
    convex = CVH_init_convexhull();
    convex_entry = malloc(sizeof(ConvexHullEntry));
    convex_entry->convex = convex;
    CVH_points_to_convex(points, convex, &reste, NULL);
    CIRCLEQ_INSERT_TAIL(convexs, convex_entry, entries);

    while (!CIRCLEQ_EMPTY(&reste)) {
        convex_entry = malloc(sizeof(ConvexHullEntry));
        convex_entry->convex = convex;
        convex = CVH_init_convexhull();
        CIRCLEQ_MOVE_TO(&reste, &points2, entries);
        CVH_points_to_convex(&points2, convex, &reste, NULL);
        CIRCLEQ_INSERT_TAIL(convexs, convex_entry, entries);
    }
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
    new_entry = GEN_new_vertex((Point) {point.x, point.y});
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
    int added = CVH_add(point, convex, reste);
    if (!added) {
        Vertex* new_entry = GEN_new_vertex_pointer(point);
        CIRCLEQ_INSERT_TAIL(reste, new_entry, entries);
    }
    // printf("Vertex(%d, %d)\n", new_entry->p->x, new_entry->p->y);
    return added;
}

/**
 * @brief Initialise un objet ConvexHull
 * @return Adresse de l'instance du polygône convexe.
 */
ConvexHull* CVH_init_convexhull(void) {
    ConvexHull* convex = calloc(1, sizeof(ConvexHull));
    if (!convex)
        return NULL;

    CIRCLEQ_INIT(&convex->poly);
    
    return convex;
}
