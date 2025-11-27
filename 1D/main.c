// We will create a 1D space where we will learn how to use structs and pointers.
// This exercise also serves as a way to get more in depth on how spaces in math work, and later on make 2D and 3D
// spaces to use physics!

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WORLD_SIZE 40
#define EPSILON .67f // EPSILON defines the fraction of the velocity the wall absorbs.
#define GRAVITY 0.2f

struct Object1D
{
    float pos;
    float velocity;
    char icon;
};

void update_physics(struct Object1D *player)
{
    player->velocity += GRAVITY;
    player->pos += player->velocity;

    // Bounce Considering the walls have infinite mass, absorbs part of the impact, and player is indestructible:
    if (player->pos <= 0.0f)
    {
        player->velocity *= -EPSILON;
        player->pos = 0.0f;
    }
    else if (player->pos >= WORLD_SIZE-1)
    {
        player->velocity *= -EPSILON;
        player->pos = WORLD_SIZE-1;
    }
}

void render(char *worldBuffer, int size, const struct Object1D *player) {
    // 1. CLEAR the buffer (loop through and set to '.')
    for (int i = 0; i < size; i++)
    {
        worldBuffer[i] = '.';
    }
    // 2. PROJECT player position (cast float to int)
    const int player_pos = (int)player->pos;
    // 3. DRAW player into buffer (handle out-of-bounds!)
    if (player_pos < 0)
    {
        worldBuffer[0] = player->icon;
    }
    else if (player_pos >= size)
    {
        worldBuffer[size-1] = player->icon;
    }
    else
    {
        worldBuffer[player_pos] = player->icon;
    }
    // 4. PRINT the buffer
    printf("\033[H");
    printf("|");

    for (int i = 0; i < size; i++)
        printf("%c", worldBuffer[i]);

    printf("|\tcurrent pos: %.2f", player->pos);
    fflush(stdout);
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

    struct Object1D Player = {.0f, input_velocity, input_icon};

    char space[WORLD_SIZE];
    struct Object1D *p_ptr = &Player;

    printf("\033[2J"); // ANSI code to Clear Screen
    while (true)
    {
        update_physics(p_ptr);
        render(space, WORLD_SIZE, p_ptr);
        usleep(16000);
    }
    return EXIT_SUCCESS;
}