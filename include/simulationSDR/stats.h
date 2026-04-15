#ifndef STATS_H
#define STATS_H

#ifdef ENABLE_STATS

/* 
 * Fonctions ne doivent pas nécessairemnt être void ou inline
 * Il faut cependant faire attention à la valeur de retour lorsque les stats sont désactivés
 */

static inline void stats_avg_latence_block(float * lat) { *lat = 0; }
static inline void stats_max_latence_block(float * lat) { *lat = 0; }
static inline void stats_min_latence_block(float * lat) { *lat = 0; }
static inline void stats_throughput_block(float * thr) { *thr = 0; }
static inline void stats_percentage_block(float * per) { *per = 0; }

#else /* ENABLE_STATS not defined: provide no-ops */

/*
 * Casts des variables en void pour éviter warnings du compilateur
 */

static inline void stats_avg_latence_block(float * lat) { (void)lat; }
static inline void stats_max_latence_block(float * lat) { (void)lat; }
static inline void stats_min_latence_block(float * lat) { (void)lat; }
static inline void stats_throughput_block(float * thr) { (void)thr; }
static inline void stats_percentage_block(float * per) { (void)per; }

#endif // ENABLE_STATS

#endif // STATS_H
