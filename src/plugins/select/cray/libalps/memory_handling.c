/*
 * Memory de-allocation
 *
 * Copyright (c) 2009-2011 Centro Svizzero di Calcolo Scientifico (CSCS)
 * Licensed under the GPLv2.
 */
#include "../basil_alps.h"

static void free_basil_processor(struct basil_node_processor *p)
{
	if (p) {
		free_basil_processor(p->next);
		xfree(p->allocation);
		xfree(p);
	}
}

static void free_basil_mem_alloc(struct basil_mem_alloc *m)
{
	if (m) {
		free_basil_mem_alloc(m->next);
		xfree(m);
	}
}

static void free_basil_memory(struct basil_node_memory *m)
{
	if (m) {
		free_basil_memory(m->next);
		free_basil_mem_alloc(m->a_head);
		xfree(m);
	}
}

static void free_basil_label(struct basil_label *l)
{
	if (l) {
		free_basil_label(l->next);
		xfree(l);
	}
}

static void free_basil_segment(struct basil_segment *s)
{
	if (s) {
		free_basil_segment(s->next);
		free_basil_processor(s->proc_head);
		free_basil_memory(s->mem_head);
		free_basil_label(s->lbl_head);
		xfree(s);
	}
}

static void free_basil_node(struct basil_node *n)
{
	if (n) {
		free_basil_node(n->next);
		free_basil_segment(n->seg_head);
		xfree(n);
	}
}

static void free_basil_rsvn_cmd(struct basil_rsvn_app_cmd *c)
{
	if (c) {
		free_basil_rsvn_cmd(c->next);
		xfree(c);
	}
}

static void free_basil_rsvn_app(struct basil_rsvn_app *a)
{
	if (a) {
		free_basil_rsvn_app(a->next);
		free_basil_rsvn_cmd(a->cmd_head);
		xfree(a);
	}
}

static void free_basil_rsvn(struct basil_rsvn *r)
{
	if (r) {
		free_basil_rsvn(r->next);
		free_basil_rsvn_app(r->app_head);
		xfree(r);
	}
}

void free_inv(struct basil_inventory *inv)
{
	if (inv) {
		if (inv->f) {
			free_basil_node(inv->f->node_head);
			free_basil_rsvn(inv->f->rsvn_head);
			xfree(inv->f);
		}
		xfree(inv);
	}
}

/*
 * Node-specifier lists
 */
void free_nodespec(struct nodespec *ns)
{
	if (ns) {
		free_nodespec(ns->next);
		xfree(ns);
	}
}

/*
 * Reservation parameters
 */
static void rsvn_free_param_mem(struct basil_memory_param *m)
{
	if (m) {
		rsvn_free_param_mem(m->next);
		xfree(m);
	}
}

static void rsvn_free_param_accel(struct basil_accel_param *a)
{
	if (a) {
		rsvn_free_param_accel(a->next);
		xfree(a);
	}
}

void rsvn_free_param(struct basil_rsvn_param *p)
{
	if (p) {
		rsvn_free_param(p->next);
		rsvn_free_param_mem(p->memory);
		rsvn_free_param_accel(p->accel);
		free_basil_label(p->labels);
		xfree(p->nodes);
		xfree(p);
	}
}

void free_rsvn(struct basil_reservation *r)
{
	if (r) {
		rsvn_free_param(r->params);
		free_nodespec(r->rsvd_nodes);
		xfree(r);
	}
}
