#ifndef RR_UI_ELEMENT_H
#define RR_UI_ELEMENT_H

#include "../point.h"
#include "cutil/vector.h"

typedef struct {
    rrPoint position;
    int active;
}rrUiElement;

typedef struct {
    cutil_vector * elements;
    int current_element;
} rrUiActiveElementGroup;

void rr_ui_active_element_group_init(rrUiActiveElementGroup* element_group);
void rr_ui_active_alement_group_uninit(rrUiActiveElementGroup* element_group);
void rr_ui_active_alement_group_add(rrUiActiveElementGroup* element_group, rrUiElement* element);
void rr_ui_active_alement_group_set(rrUiActiveElementGroup* element_group, int index);
void rr_ui_active_alement_group_next(rrUiActiveElementGroup* element_group);
void rr_ui_active_alement_group_previous(rrUiActiveElementGroup* element_group);



#endif
