#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 400
#define TILE 40
#define NUM_OBS 5

typedef struct { int x; int y; } Posicion;

typedef struct {
    Posicion jugador, moneda, obstaculos[NUM_OBS];
    int vidas, puntaje;    
} Juego;

typedef enum { ARRIBA, ABAJO, IZQUIERDA, DERECHA, SALIR } Direccion;


void inicializar(Juego *j) {
    srand(time(NULL));
    j->jugador.x = WIDTH / (2 * TILE);
    j->jugador.y = HEIGHT / (2 * TILE);
    j->moneda.x = rand() % (WIDTH / TILE);
    j->moneda.y = rand() % (HEIGHT / TILE);
    j->vidas = 3;
    j->puntaje = 0;

    for (int i = 0; i < NUM_OBS; i++) {
        do {
            j->obstaculos[i].x = rand() % (WIDTH / TILE);
            j->obstaculos[i].y = rand() % (HEIGHT / TILE);
        } while (j->obstaculos[i].x == j->jugador.x && j->obstaculos[i].y == j->jugador.y);
    }
}

void mover(Juego *j, SDL_Keycode key) {
    switch (key) {
        case SDLK_w: if (j->jugador.y > 0) j->jugador.y--; break;
        case SDLK_s: if (j->jugador.y < HEIGHT / TILE - 1) j->jugador.y++; break;
        case SDLK_a: if (j->jugador.x > 0) j->jugador.x--; break;
        case SDLK_d: if (j->jugador.x < WIDTH / TILE - 1) j->jugador.x++; break;
    }
}

// cierre de cambios con sdl //
void mostrarTablero(Juego *j, int ancho, int alto) {
    system("clear");
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            if (x == j->jugador.x && y == j->jugador.y)
                printf("☺");
            else if (x == j->moneda.x && y == j->moneda.y)
                printf("$");
            else {
                int esObstaculo = 0;
                for (int k = 0; k < 5; k++) {
                    if (x == j->obstaculos[k].x && y == j->obstaculos[k].y) {
                        printf("#");
                        esObstaculo = 1;
                        break;
                    }
                }
                if (!esObstaculo) printf(".");
            }
        }
        printf("\n");
    }
    printf("\nPuntaje: %d  |  Vidas: %d\n", j->puntaje, j->vidas);
    printf("Mover: w/a/s/d | Salir: x\n");
}

Direccion leerEntradaUsuario() {
    char input = getch();
    switch (input) {
        case 'w': return ARRIBA;
        case 's': return ABAJO;
        case 'a': return IZQUIERDA;
        case 'd': return DERECHA;
        case 'x': return SALIR;
        default:  return -1;
    }
}

void moverJugador(Juego *j, Direccion dir, int ancho, int alto) {
    switch (dir) {
        case ARRIBA:    if (j->jugador.y > 0) j->jugador.y--; break;
        case ABAJO:     if (j->jugador.y < alto-1) j->jugador.y++; break;
        case IZQUIERDA: if (j->jugador.x > 0) j->jugador.x--; break;
        case DERECHA:   if (j->jugador.x < ancho-1) j->jugador.x++; break;
        default: break;
    }
}

void verificarColision(Juego *j, int ancho, int alto) {
    // Moneda
    if (j->jugador.x == j->moneda.x && j->jugador.y == j->moneda.y) {
        j->puntaje++;
        j->moneda.x = rand() % ancho;
        j->moneda.y = rand() % alto;
    }

    // Obstáculos
    for (int i = 0; i < 5; i++) {
        if (j->jugador.x == j->obstaculos[i].x && j->jugador.y == j->obstaculos[i].y) {
            j->vidas--;
            printf("\n¡Cuidado! Has perdido una vida. Vidas restantes: %d\n", j->vidas);
            j->jugador.x = ancho / 2;
            j->jugador.y = alto / 2;
            if (j->vidas <= 0) {
                printf("\nHas perdido todas tus vidas. Fin del juego.\n");
                exit(0);
            }
        }
    }
}

int main() {
    const int ancho = 20, alto = 10;
    Juego juego;
    Direccion dir;
    inicializarJuego(&juego, ancho, alto);

    while (1) {
        mostrarTablero(&juego, ancho, alto);
        dir = leerEntradaUsuario();
        if (dir == SALIR) break;
        if (dir != -1) {
            moverJugador(&juego, dir, ancho, alto);
            verificarColision(&juego, ancho, alto);
        }
    }

    printf("\nJuego terminado. Puntaje final: %d\n", juego.puntaje);
    return 0;
}
