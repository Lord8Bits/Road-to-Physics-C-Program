// We will create a 1D space where we will learn how to use structs and pointers.
// This exercise also serves as a way to get more in depth on how spaces in math work, and later on make 2D and 3D
// spaces to use physics!

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WORLD_SIZE 40
#define EPSILON 0.67f // EPSILON defines the fraction of the velocity the wall absorbs.
#define GRAVITY 0.0f

struct Object1D
{
    float pos; // Position of the object in the x-axis
    float velocity; // Velocity vector
    float mass;
    char icon; // Icon to represent the object
};

// Physics function that updates the player position as well as the velocity, gravity's influence and impact absorption.
void update_physics(struct Object1D *object)
{
    object->velocity += GRAVITY; // Gravity > 0 falls to the right, Gravity < 0 falls to the left.
    object->pos += object->velocity; // Updates position by velocity

    // Bounce Considering the walls have infinite mass, absorbs part of the impact, and player is indestructible:
    if (object->pos <= 0.0f)
    {
        object->velocity *= -EPSILON; // Velocity after collision with an object of infinite mass equals to initial_velocity * (-epsilon), epsilon represents the impact absorption factor.
        object->pos = 0.0f;
    }
    else if (object->pos >= WORLD_SIZE-1)
    {
        object->velocity *= -EPSILON;
        object->pos = WORLD_SIZE-1;
    }
}

void object_collision(struct Object1D *object1, struct Object1D *object2)
{
    if (fabsf(object1->pos - object2->pos) <= 0.5f)
    {
        const float obj_mass1 = object1->mass;
        const float obj_mass2 = object2->mass;
        const float obj_velo1 = object1->velocity;
        const float obj_velo2 = object2->velocity;

        object1->velocity = ((obj_mass1 - obj_mass2)*obj_velo1 + (2 * obj_mass2)*obj_velo2)/(obj_mass1 + obj_mass2);
        object2->velocity = ((obj_mass2 - obj_mass1)*obj_velo2 + (2 * obj_mass1)*obj_velo1)/(obj_mass1 + obj_mass2);
    }
}

void render(char *worldBuffer, int size, const struct Object1D *object) {
    // 1. CLEAR the buffer (loop through and set to '.')
    for (int i = 0; i < size; i++)
    {
        worldBuffer[i] = '.';
    }
    // 2. PROJECT player position (cast float to int)
    const int player_pos = (int)object->pos;
    // 3. DRAW player into buffer (handle out-of-bounds!)
    if (player_pos < 0)
    {
        worldBuffer[0] = object->icon;
    }
    else if (player_pos >= size)
    {
        worldBuffer[size-1] = object->icon;
    }
    else
    {
        worldBuffer[player_pos] = object->icon;
    }
    // 4. PRINT the buffer
    printf("\033[H"); // Returns to the beginning of the line to overwrite the previous output.
    printf("|");

    for (int i = 0; i < size; i++)
        printf("%c", worldBuffer[i]);

    printf("|\tcurrent pos: %.2f", object->pos);
    fflush(stdout); // Clears the buffer
}

int main(void)
{
    float input_velocity;
    char input_icon;
    printf("Enter the desired velocity for the object: ");
    scanf("%f", &input_velocity);

    printf("Enter the desired icon for the object: ");
    scanf(" %c", &input_icon);
    fflush(stdout);

    struct Object1D object = {0.0f, input_velocity, 20.0f, input_icon};

    char space[WORLD_SIZE]; // The world in which our object will move.
    struct Object1D *player_ptr = &object;

    printf("\033[2J"); // ANSI code to Clear Screen
    while (true)
    {
        update_physics(player_ptr);
        render(space, WORLD_SIZE, player_ptr);
        usleep(16000); // 62.5 FPS = 1/0.016s
    }
    return EXIT_SUCCESS;
}