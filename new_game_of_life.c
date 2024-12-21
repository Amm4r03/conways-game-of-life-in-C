#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

// define what dead and alive cells should look like
const char dead = ' ';
const char alive = '@';

void print_grid(char** grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {        
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

char** create_grid(int rows, int cols, int** starting_points, int num_starting_points) {
    char** grid = (char**) malloc(rows * sizeof(char*));
    if (!grid) {
        fprintf(stderr, "ERROR : malloc failed for creating grid\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        grid[i] = (char*) malloc(cols * sizeof(char));
        if (!grid[i]) {
            fprintf(stderr, "ERROR :malloc failed\n");
            exit(1);
        }
    }

    // initialise values to dead
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = dead;
        }
    }

    // set cells to alive from starting points
    if (starting_points != NULL && num_starting_points > 0) {
        for (int i = 0; i < num_starting_points; i++) {
           int x = starting_points[i][0];
            int y = starting_points[i][1];
            grid[x][y] = alive;
        }
    }

    return grid;
}

int count_living_neighbors(char** grid, int rows, int cols, int curr_row, int curr_col) {
    int living_count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; 
            int neighbor_row = curr_row + i;
            int neighbor_col = curr_col + j;

            if (neighbor_row >= 0 && neighbor_row < rows &&
                neighbor_col >= 0 && neighbor_col < cols &&
                grid[neighbor_row][neighbor_col] == alive) {
                living_count++;
            }
        }
    }
    return living_count;
}

/*
Any live cell with fewer than two live neighbours dies, as if by underpopulation.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

char** compute_grid(char** grid, int rows, int cols) {
    char** new_grid = create_grid(rows, cols, NULL, 0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char current_state = grid[i][j];
            int living_neighbors = count_living_neighbors(grid, rows, cols, i, j);

            if (current_state == alive) {
                new_grid[i][j] = (living_neighbors == 2 || living_neighbors == 3) ? alive : dead;
            } else {
                new_grid[i][j] = (living_neighbors == 3) ? alive : dead;
            }
        }
    }
    return new_grid;
}

int get_random_int(int max_val) {
    return (rand() % max_val);
}

int** get_starting_points(int num_rows, int num_cols, int count) {
    int** points = (int**) malloc(count * sizeof(int*));
    for (int i = 0; i < count; i++) {
        points[i] = (int*) malloc(2 * sizeof(int));

        points[i][0] = get_random_int(num_rows);
        points[i][1] = get_random_int(num_cols);
    }
    return points;
}

void free_grid(char** grid, int rows) {
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

int identical_grids(char** grid1, char** grid2, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid1[i][j] != grid2[i][j]) {
                return 0; 
            }
        }
    }
    return 1;
}

void run_simulation(char** grid, int num_rows, int num_cols, int generations) {
    for (int i = 0; i < generations; i++) {
        // Sleep(250);
        // printf("Generation %d of %d\n", i + 1, generations);
        print_grid(grid, num_rows, num_cols);

        // Compute the next generation
        char** new_grid = compute_grid(grid, num_rows, num_cols);
        
        if (identical_grids(new_grid, grid, num_rows, num_cols)) {
            printf("identical grids found - exiting\n");
            free_grid(grid, num_rows);
            break;
        }
        
        // Free the old grid and update the pointer
        free_grid(grid, num_rows);
        grid = new_grid;
        system("clear");
    }
    
    printf("\nSimulation complete\n\n");

    // Final cleanup
    free_grid(grid, num_rows);
    printf("freed all memory");
}



int main() {
    srand(time(NULL));

    // simulation parameters
    int rows = 26;
    int cols = 75;
    int count_starting_points = 1000;
    int generations = 500;
    int** testing_starting_points = get_starting_points(rows, cols, count_starting_points);

    // char** grid = create_grid(rows, cols, starting_points, sizeof(starting_points) / sizeof(starting_points[0]));
    char** grid = create_grid(rows, cols, testing_starting_points, count_starting_points);
    // print_grid(grid, rows, cols);
    run_simulation(grid, rows, cols, generations);

    for (int i = 0; i < count_starting_points; i++) {
        free(testing_starting_points[i]);
    }
    free(testing_starting_points);

    return 0;
}