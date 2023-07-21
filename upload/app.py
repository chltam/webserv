#! /usr/bin/python3

import pygame
import random

# Initialize pygame
pygame.init()

# Set up the game window
width, height = 400, 900
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Tetris")

# Define colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
CYAN = (0, 255, 255)
YELLOW = (255, 255, 0)
MAGENTA = (255, 0, 255)
ORANGE = (255, 165, 0)
BLUE = (0, 0, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

# Define tetromino shapes
SHAPES = [
    [[1, 1, 1, 1]],  # I
    [[1, 1], [1, 1]],  # O
    [[1, 1, 0], [0, 1, 1]],  # Z
    [[0, 1, 1], [1, 1, 0]],  # S
    [[1, 1, 1], [0, 1, 0]],  # T
    [[1, 1, 1], [0, 0, 1]],  # L
    [[1, 1, 1], [1, 0, 0]]  # J
]

# Set up the game grid
grid_size = 30
grid_width = width // grid_size
grid_height = height // grid_size
grid = [[BLACK] * grid_width for _ in range(grid_height)]


def draw_grid():
    for y in range(grid_height):
        for x in range(grid_width):
            pygame.draw.rect(screen, grid[y][x], (x * grid_size, y * grid_size, grid_size, grid_size))


def draw_tetromino(tetromino, x, y, color):
    for row in range(len(tetromino)):
        for col in range(len(tetromino[row])):
            if tetromino[row][col]:
                pygame.draw.rect(screen, color, ((x + col) * grid_size, (y + row) * grid_size, grid_size, grid_size))


def check_collision(tetromino, x, y):
    for row in range(len(tetromino)):
        for col in range(len(tetromino[row])):
            if tetromino[row][col]:
                if x + col < 0 or x + col >= grid_width or y + row >= grid_height or grid[y + row][x + col] != BLACK:
                    return True
    return False


def clear_rows():
    full_rows = []
    for row in range(grid_height):
        if all(cell != BLACK for cell in grid[row]):
            full_rows.append(row)

    for row in full_rows:
        del grid[row]
        grid.insert(0, [BLACK] * grid_width)


def game_over():
    font = pygame.font.Font(None, 70)
    text = font.render("Game Over", True, RED)
    screen.blit(text, (width // 2 - text.get_width() // 2, height // 2 - text.get_height() // 2))
    pygame.display.flip()
    pygame.time.wait(2000)


def rotate_tetromino(tetromino):
    return list(zip(*reversed(tetromino)))


def tetris():
    clock = pygame.time.Clock()
    fps = 10

    tetromino = random.choice(SHAPES)
    tetromino_color = random.choice([CYAN, YELLOW, MAGENTA, ORANGE, BLUE, GREEN, RED])
    tetromino_x = grid_width // 2 - len(tetromino[0]) // 2
    tetromino_y = 0

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    if not check_collision(tetromino, tetromino_x - 1, tetromino_y):
                        tetromino_x -= 1
                elif event.key == pygame.K_RIGHT:
                    if not check_collision(tetromino, tetromino_x + 1, tetromino_y):
                        tetromino_x += 1
                elif event.key == pygame.K_DOWN:
                    if not check_collision(tetromino, tetromino_x, tetromino_y + 1):
                        tetromino_y += 1
                elif event.key == pygame.K_UP:
                    rotated = rotate_tetromino(tetromino)
                    if not check_collision(rotated, tetromino_x, tetromino_y):
                        tetromino = rotated

        if not check_collision(tetromino, tetromino_x, tetromino_y + 1):
            tetromino_y += 1
        else:
            for row in range(len(tetromino)):
                for col in range(len(tetromino[row])):
                    if tetromino[row][col]:
                        grid[tetromino_y + row][tetromino_x + col] = tetromino_color

            clear_rows()

            if tetromino_y == 0:
                game_over()
                return

            tetromino = random.choice(SHAPES)
            tetromino_color = random.choice([CYAN, YELLOW, MAGENTA, ORANGE, BLUE, GREEN, RED])
            tetromino_x = grid_width // 2 - len(tetromino[0]) // 2
            tetromino_y = 0

        screen.fill(BLACK)
        draw_grid()
        draw_tetromino(tetromino, tetromino_x, tetromino_y, tetromino_color)
        pygame.display.flip()
        clock.tick(fps)


# Start the game
tetris()
