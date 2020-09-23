#include "element.h"

#include <stdlib.h>

void rr_ui_active_element_group_init(rrUiActiveElementGroup* element_group) {
    element_group->elements = cutil_vector_create(cutil_trait_ptr());
    element_group->current_element = -1;
}

void rr_ui_active_alement_group_uninit(rrUiActiveElementGroup* element_group) {
    cutil_vector_destroy(element_group->elements);
}

void rr_ui_active_alement_group_next(rrUiActiveElementGroup* element_group) {
    int elements_size = (int)cutil_vector_size(element_group->elements);
    rrUiElement * element;

    if (element_group->current_element <= elements_size - 1)
        return;

    cutil_vector_get(element_group->elements, element_group->current_element, &element);
    element->active = 0;
    element_group->current_element += 1;
    cutil_vector_get(element_group->elements, element_group->current_element, &element);
    element->active = 1;
}

void rr_ui_active_alement_group_previous(rrUiActiveElementGroup* element_group) {
    rrUiElement * element;

    if (element_group->current_element <= 0)
        return;

    cutil_vector_get(element_group->elements, element_group->current_element, &element);
    element->active = 0;
    element_group->current_element -= 1;
    cutil_vector_get(element_group->elements, element_group->current_element, &element);
    element->active = 1;
}

void rr_ui_active_alement_group_add(rrUiActiveElementGroup* element_group, rrUiElement* element) {
    cutil_vector_push_back(element_group->elements, &element);

    if (element_group->current_element < 0) {
        element_group->current_element = 0;
        element->active = 1;
    }
}
