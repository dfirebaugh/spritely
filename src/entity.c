#include "sprite_sheet.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct Entity_manager *Entity_manager_t;

typedef struct coordinates {
  uint8_t x;
  uint8_t y;
} coordinates_t;

struct Entity {
  Sprite_sheet_t sprite_sheet;
  uint8_t sprite_index;
  coordinates_t coordinates;
  uint8_t is_visible;
  uint8_t position;
  struct Entity *previous;
  struct Entity *next;
};

void Entity_manager_free_future_entitys(Entity_manager_t manager);

struct Entity_manager {
  struct Entity *entity;
};

Entity_manager_t Entity_manager_make(void) {
  Entity_manager_t new_entity_manager = malloc(sizeof(struct Entity_manager));
  new_entity_manager->entity = NULL;

  return new_entity_manager;
}

void Entity_free(Entity_manager_t manager) {
  if (manager->entity) {
    while (manager->entity->previous != NULL)
      manager->entity = manager->entity->previous;

    Entity_manager_free_future_entitys(manager);
    free(manager->entity);
    manager->entity = NULL;
  }

  free(manager);
  manager = NULL;
}

void Entity_render(Entity_manager_t manager) {
  if (manager->entity == NULL)
    return;

  struct Entity *entity_iterator = manager->entity;
  while (entity_iterator != NULL) {
    Sprite_sheet_render_sprite(
        entity_iterator->sprite_sheet, entity_iterator->sprite_index,
        entity_iterator->coordinates.x, entity_iterator->coordinates.y);
    entity_iterator = entity_iterator->previous;
  }
}

void Entity_set_position(Entity_manager_t manager, uint8_t x, uint8_t y,
                         uint8_t position) {
#if 0
    entity->position.x = x;
    entity->position.y = y;
#endif
}

void Entity_manager_free_future_entitys(Entity_manager_t manager) {
  if (manager->entity == NULL || manager->entity->next == NULL)
    return;

  struct Entity *entity_iterator = manager->entity->next;
  while (entity_iterator != NULL) {
    struct Entity *entity_to_free = entity_iterator;
    entity_iterator = entity_iterator->next;
    free(entity_to_free);
  }
  manager->entity->next = NULL;
}

void Entity_manager_new_entity(Entity_manager_t manager,
                               Sprite_sheet_t sprite_sheet,
                               uint8_t sprite_index, uint8_t x, uint8_t y) {
  Entity_manager_free_future_entitys(manager);

  struct Entity *new_entity = (struct Entity *)malloc(sizeof(struct Entity));
  if (!new_entity)
    return;

  new_entity->sprite_sheet = sprite_sheet;
  new_entity->sprite_index = sprite_index;
  // new_entity->position = position;
  new_entity->coordinates.x = x;
  new_entity->coordinates.y = y;
  new_entity->previous = manager->entity;
  new_entity->next = NULL;

  if (manager->entity)
    manager->entity->next = new_entity;

  manager->entity = new_entity;
}
