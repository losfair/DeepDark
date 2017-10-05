#ifndef _ICE_API_VIEW_H_
#define _ICE_API_VIEW_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vIceHtmlTemplateEngine {};
typedef struct vIceHtmlTemplateEngine * IceHtmlTemplateEngine;

IceHtmlTemplateEngine ice_view_html_template_create_engine(
    const char *name
);
void ice_view_html_template_destroy_engine(
    IceHtmlTemplateEngine engine
);
ice_uint8_t ice_view_html_template_add(
    IceHtmlTemplateEngine engine,
    const char *name,
    const char *content
);
ice_uint8_t ice_view_html_template_add_file(
    IceHtmlTemplateEngine engine,
    const char *path
);
ice_owned_string_t ice_view_html_template_render_json_to_owned(
    IceHtmlTemplateEngine engine,
    const char *name,
    const char *data
);

#ifdef __cplusplus
}
#endif

#endif
