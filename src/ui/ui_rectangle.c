#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_local.h"

#include "graphics_manager.h"

#include "ui/ui_rectangle.h"

ui_rectangle* ui_rectangle_new() {

  ui_rectangle* rect = malloc(sizeof(ui_rectangle));
  
  rect->top_left = v2(10, 10);
  rect->bottom_right = v2(20, 20);
  rect->color = v4_white();
  
  rect->texture = NULL;

  rect->border_size = 0.0;
  rect->border_color = v4_black();
  
  rect->active = true;
  
  return rect;
  
}

void ui_rectangle_delete(ui_rectangle* rect) {
  free(rect);
}

void ui_rectangle_set_texture(ui_rectangle* rect, texture* t) {
  rect->texture = t;
}

void ui_rectangle_update(ui_rectangle* rect) {
  
}

void ui_rectangle_move(ui_rectangle* rect, vector2 pos) {
  vector2 size = v2_sub(rect->bottom_right, rect->top_left);
  rect->top_left = pos;
  rect->bottom_right = v2_add(pos, size);
}

void ui_rectangle_resize(ui_rectangle* rect, vector2 size) {
  rect->bottom_right = v2_add(rect->top_left, size);
}

void ui_rectangle_render(ui_rectangle* rect) {
  
  if(!rect->active) {
    return;
  }
  
	glMatrixMode(GL_PROJECTION);
  glPushMatrix();
	glLoadIdentity();
	glOrtho(0, graphics_viewport_width(), graphics_viewport_height(), 0, -1, 1);
  
	glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
	glLoadIdentity();
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  if(rect->texture != NULL) {
    glActiveTexture(GL_TEXTURE0 + 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_handle(rect->texture) );
  }
  
  glColor4f(rect->color.r, rect->color.g, rect->color.b, rect->color.a);  
    
  glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(rect->top_left.x, rect->top_left.y, 0);
    glTexCoord2f(1, 1); glVertex3f(rect->bottom_right.x, rect->top_left.y, 0);
    glTexCoord2f(1, 0); glVertex3f(rect->bottom_right.x, rect->bottom_right.y, 0);
    glTexCoord2f(0, 0); glVertex3f(rect->top_left.x, rect->bottom_right.y, 0);
  glEnd();
  
  if(rect->border_size > 0) {
  
    glColor4f(rect->border_color.r, rect->border_color.g, rect->border_color.b, rect->border_color.a);  
    glLineWidth(rect->border_size);
    
    glBegin(GL_LINE_STRIP);
      
      glVertex3f(rect->top_left.x, rect->top_left.y, 0);
      glVertex3f(rect->bottom_right.x, rect->top_left.y, 0);
      glVertex3f(rect->bottom_right.x, rect->bottom_right.y, 0);
      glVertex3f(rect->top_left.x, rect->bottom_right.y, 0);
      glVertex3f(rect->top_left.x, rect->top_left.y, 0);
      
    glEnd();
  
    glLineWidth(1);
  }
  
  glColor4f(1, 1, 1, 1);
  
  glDisable(GL_BLEND);
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
	glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
  if(rect->texture != NULL) {
    glActiveTexture(GL_TEXTURE0 + 0);
    glDisable(GL_TEXTURE_2D);
  }
 
}

bool ui_rectangle_contains_position(ui_rectangle* rect, vector2 pos) {

  if (!rect->active) { return false; }

  if(( pos.x > rect->top_left.x ) && ( pos.x < rect->bottom_right.x ) &&
     ( pos.y > rect->top_left.y ) && ( pos.y < rect->bottom_right.y )) {
    return true;
  } else {
    return false;
  }
}
