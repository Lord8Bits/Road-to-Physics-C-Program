// We will create a 1D space where we will learn how to use structs and pointers.
// This exercise also serves as a way to get more in depth on how spaces in math work, and later on make 2D and 3D
// spaces to use physics!
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WORLD_SIZE 200
#define EPSILON 1 // EPSILON defines the fraction of the velocity the wall absorbs.
#define GRAVITY 0.0
#define SUB_STEPS 100000.0
int n_collision = 0;

struct Object1D
{
    double pos; // Position of the object in the x-axis
    double velocity; // Velocity vector
    double mass;
    char icon; // Icon to represent the object
};

void object_collision(struct Object1D *object1, struct Object1D *object2)
{
    const double dist = object1->pos - object2->pos;
    if (fabs(dist) <= 0.5)
    {
        const double obj_velo1 = object1->velocity;
        const double obj_velo2 = object2->velocity;

        const double relative_velo = obj_velo1 - obj_velo2;
        /* If obj1 moves to the left and is to the left then no collision.
         * If it is to the left, and it moves at the opposite direction (so negative * positive) then there is a collision and vice versa.
         */
        if (relative_velo * dist < 0.0)
        {
            n_collision += 1;
            const double obj_mass1 = object1->mass;
            const double obj_mass2 = object2->mass;

            object1->velocity = ((obj_mass1 - obj_mass2)*obj_velo1 + (2 * obj_mass2)*obj_velo2)/(obj_mass1 + obj_mass2);
            object2->velocity = ((obj_mass2 - obj_mass1)*obj_velo2 + (2 * obj_mass1)*obj_velo1)/(obj_mass1 + obj_mass2);
        }
    }
}

// Physics function that updates the player position as well as the velocity, gravity's influence, impact absorption and Object collision.
void update_physics(struct Object1D *Objects, int object_count)
{
    // We move the "Time" forward in small slices
    constexpr double dt = 1.0 / SUB_STEPS;

    for (int step = 0; step < SUB_STEPS; step++)
    {
        // 1. Move EVERY object a tiny bit
        for (int i = 0; i < object_count; i++)
        {
            struct Object1D *obj = &Objects[i];

            // Apply Gravity (Scaled by the tiny time step)
            obj->velocity += GRAVITY * dt;

            // Move only a fraction of the total velocity
            obj->pos += obj->velocity * dt;

            // 2. Check Wall Collision immediately
            if (obj->pos <= 0.0) {
                n_collision += 1;
                obj->velocity *= -EPSILON;
                obj->pos = 0.0;
            }
            else if (obj->pos >= WORLD_SIZE - 1) {
                obj->velocity *= -EPSILON;
                obj->pos = WORLD_SIZE - 1;
            }
        }
        for (int i = 0; i < object_count-1; i++)
        {
            struct Object1D *obj1 = &Objects[i];

            for (int j = i+1; j < object_count; j++)
            {
                struct Object1D *obj2 = &Objects[j];
                object_collision(obj1, obj2);
            }
        }
    }
}

void render(char *worldBuffer, int size, const struct Object1D *Objects, int object_count) {
    // 1. CLEAR the buffer (loop through and set to '.')
    for (int i = 0; i < size; i++)
        worldBuffer[i] = '.';

    for (int i = 0; i < object_count; i++)
    {
        const struct Object1D *current = &Objects[i];

        // 2. PROJECT player position (cast float to int)
        const int player_pos = (int)current->pos;

        // 3. DRAW player into buffer (handle out-of-bounds!)
        if (player_pos < 0)
        {
            worldBuffer[0] = current->icon;
        }
        else if (player_pos >= size)
        {
            worldBuffer[size-1] = current->icon;
        }
        else
        {
            worldBuffer[player_pos] = current->icon;
        }
    }

    // 4. PRINT the buffer
    printf("\033[H"); // Returns to the beginning of the line to overwrite the previous output.
    printf("|");

    for (int i = 0; i < size; i++)
        printf("%c", worldBuffer[i]);
    printf("|\tNumber of collisions is: %d", n_collision);

    fflush(stdout); // Clears the buffer
}

int main(void)
{
    double input_velocity;
    char input_icon;
    printf("Enter the desired velocity for the object: ");
    scanf("%lf", &input_velocity);

    printf("Enter the desired icon for the object: ");
    scanf(" %c", &input_icon);
    fflush(stdout);

    char space[WORLD_SIZE]; // The world in which our object will move.

    struct Object1D Objects[2];
    struct Object1D object1 = {30.0f, input_velocity, pow(100.0, 1), input_icon};
    struct Object1D object2 = {20.0f, 0.0f, 100.0f, 'o'};

    Objects[0] = object1;
    Objects[1] = object2;

    printf("\033[2J"); // ANSI code to Clear Screen
    while (true)
    {
        update_physics(Objects, 2);
        render(space, WORLD_SIZE, Objects, 2);
        usleep(16000);
    }
    return EXIT_SUCCESS;
}