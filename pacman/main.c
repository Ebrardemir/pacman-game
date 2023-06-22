#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define BLOK_BOYUTU 20
#define LABIRENT_GENISLIK 21
#define LABIRENT_YUKSEKLIK 17
#define MAKS_HAYALET_SAYISI 5


char labirent[LABIRENT_YUKSEKLIK][LABIRENT_GENISLIK] = {  //labirent çizimi
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', '#', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', ' ', ' ', '#'},
    {'#', '#', '#', ' ', '#', '#', '#', '#', ' ', '#', ' ', '#', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#'},
    {'#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', ' ', ' ', ' ', '#'},
    {'#', '#', ' ', '#', ' ', '#', '#', ' ', '#', '#', '#', '#', ' ', '#', ' ', ' ', '#', '#', ' ', '#', '#'},
    {'#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', '#', '#', ' ', '#', '#'},
    {'#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', '#', ' ', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', ' ', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

int EKRAN_GENISLIGI = BLOK_BOYUTU * LABIRENT_GENISLIK;
int EKRAN_YUKSEKLIGI = BLOK_BOYUTU * LABIRENT_YUKSEKLIK;

int pacmanX = 1; // Pacman'in başlangıç konumu x koordinatı
int pacmanY = 1; // Pacman'in başlangıç konumu y koordinatı

int hayaletSayisi = 5; // Hayalet sayısı
int hayaletX[MAKS_HAYALET_SAYISI] = {8, 10, 12, 6, 14}; // Hayaletlerin başlangıç konumu x koordinatları
int hayaletY[MAKS_HAYALET_SAYISI] = {8, 9, 10, 7, 12}; // Hayaletlerin başlangıç konumu y koordinatları

bool yem[LABIRENT_YUKSEKLIK][LABIRENT_GENISLIK]; // Yemlerin durumunu tutan 2D dizi

int skor = 0; // Skor değişkeni

void labirentCiz(SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.w = BLOK_BOYUTU;
    rect.h = BLOK_BOYUTU;

    for (int i = 0; i < LABIRENT_YUKSEKLIK; i++) {
        for (int j = 0; j < LABIRENT_GENISLIK; j++) {
            rect.x = j * BLOK_BOYUTU;
            rect.y = i * BLOK_BOYUTU;

            if (labirent[i][j] == '#') {  //duvari temsil ediyorsa
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  //mavi renk eklenir
                SDL_RenderFillRect(renderer, &rect);  //dikdörtgen çizilir
            } else if (yem[i][j]) {  //eger yemi temsil ediyorsa
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //sari renk eklenir
                SDL_RenderDrawPoint(renderer, rect.x + BLOK_BOYUTU / 2, rect.y + BLOK_BOYUTU / 2); //rect yapısının merkezine bir nokta çizilir
            }
        }
    }

    // Pacman karakterini çiz
    SDL_Rect pacman;
    pacman.x = pacmanX * BLOK_BOYUTU;  // Pacman karakterinin konumunu piksel cinsinden belirtir
    pacman.y = pacmanY * BLOK_BOYUTU;
    pacman.w = BLOK_BOYUTU;
    pacman.h = BLOK_BOYUTU;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  //pacman karakterinin rengini sari hale getirme
    SDL_RenderFillRect(renderer, &pacman);  //Pacman karakterinin çizimi

    // Hayaletleri çiz
    SDL_Rect hayalet;
    hayalet.w = BLOK_BOYUTU;  // her hayaletin konumunu piksel cinsinden belirtmek için
    hayalet.h = BLOK_BOYUTU;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  //çizim rengi kırmızı olarak ayarlanır
    for (int i = 0; i < hayaletSayisi; i++) {
        hayalet.x = hayaletX[i] * BLOK_BOYUTU;
        hayalet.y = hayaletY[i] * BLOK_BOYUTU;
        SDL_RenderFillRect(renderer, &hayalet);  //hayaletler çizilir
    }
}

bool carpismaKontrol(int x, int y) {  //duvarla karsilasip -karsilasmama kontrolu
    return (labirent[y][x] == '#');
}

bool hayaletCarpismaKontrolu() {  //hayalet ile pacmanin carpisma kontrolu
    for (int i = 0; i < hayaletSayisi; i++) {
        if (pacmanX == hayaletX[i] && pacmanY == hayaletY[i]) {
            return true; //true degeri dondurur yani karsilastilar
        }
    }
    return false; //karsilasmadiklari zaman false degeri doner
}

void hayaletHareketi() {
    for (int i = 0; i < hayaletSayisi; i++) {  //hayalet sayisina gore dongu olusturma
        int yon = rand() % 4;  // Hayaletin yeni hareket yonunu rastgele seçme

        switch (yon) {
            case 0: // yukari
                if (!carpismaKontrol(hayaletX[i], hayaletY[i] - 1)) {
                    hayaletY[i]--;
                }
                break;
            case 1: // asagi
                if (!carpismaKontrol(hayaletX[i], hayaletY[i] + 1)) {
                    hayaletY[i]++;
                }
                break;
            case 2: // sola
                if (!carpismaKontrol(hayaletX[i] - 1, hayaletY[i])) {
                    hayaletX[i]--;
                }
                break;
            case 3: // saga
                if (!carpismaKontrol(hayaletX[i] + 1, hayaletY[i])) {
                    hayaletX[i]++;
                }
                break;
        }

        // Hayaletin sinirli bir bolgede dolanması
        if (hayaletX[i] <= 1 || hayaletX[i] >= LABIRENT_GENISLIK - 2 || hayaletY[i] <= 1 || hayaletY[i] >= LABIRENT_YUKSEKLIK - 2) { //hayaletlerin rastgele hareket etmesini ve sinirli bir bolgede dolanmasini saglar
            // Hayaletin hareket yönünü rastgele degistirme
            yon = rand() % 4;
            switch (yon) {
                case 0: // yukari
                    if (!carpismaKontrol(hayaletX[i], hayaletY[i] - 1)) {
                        hayaletY[i]--;
                    }
                    break;
                case 1: // asagi
                    if (!carpismaKontrol(hayaletX[i], hayaletY[i] + 1)) {
                        hayaletY[i]++;
                    }
                    break;
                case 2: // sola
                    if (!carpismaKontrol(hayaletX[i] - 1, hayaletY[i])) {
                        hayaletX[i]--;
                    }
                    break;
                case 3: // saga
                    if (!carpismaKontrol(hayaletX[i] + 1, hayaletY[i])) {
                        hayaletX[i]++;
                    }
                    break;
            }
        }
    }
    SDL_Delay(200); // hayaletlerin hareket süresini kontrol etmek için
}

void hayaletBaslangic() {
    // Hayaletlerin farkli ve birbirinden uzak noktalarda baslamasi icin
    hayaletX[0] = 2;
    hayaletY[0] = LABIRENT_YUKSEKLIK - 2;

    hayaletX[1] = LABIRENT_GENISLIK/ 2;
    hayaletY[1] = 2;

    hayaletX[2] = LABIRENT_GENISLIK - 2;
    hayaletY[2] = LABIRENT_YUKSEKLIK - 2;

    hayaletX[3] = LABIRENT_GENISLIK / 3;
    hayaletY[3] = LABIRENT_YUKSEKLIK / 3;

    hayaletX[4] = LABIRENT_GENISLIK - 4;
    hayaletY[4] = LABIRENT_YUKSEKLIK / 2;
}

void yemBaslangic() {
    for (int i = 0; i < LABIRENT_YUKSEKLIK; i++) {
        for (int j = 0; j < LABIRENT_GENISLIK; j++) {
            if (labirent[i][j] == ' ') {
                yem[i][j] = true;  // Labirentte bos olan kisimlara yem eklemek icin
            } else {
                yem[i][j] = false;  // Duvarlar varsa yem eklenmemesi icin false atanir
            }
        }
    }
}

int main(int argc, char* args[]) {
    srand(time(NULL));  //  her seferinde farkli rastgele degerler almak icin

    SDL_Window* window = NULL; //pencere olusturma
    SDL_Renderer* renderer = NULL; //renderer olusturma

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {  // SDL'in baslatilip baslatilmadiginin kontrolu
        printf("SDL başlatilamadi! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, EKRAN_GENISLIGI, EKRAN_YUKSEKLIGI, SDL_WINDOW_SHOWN);//pencerinin baslatilip baslatilmediginin kontrolu
    if (window == NULL) {
        printf("window başlayilamadi! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //renderer kontrolu
    if (renderer == NULL) {
        printf("Renderer başlatilamadi! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    hayaletBaslangic();
    yemBaslangic();

    bool quit = false; //oyun döngüsünün devam edip etmeyecegini kontrol etmek için
    SDL_Event e; //klavye girisleri veya pencere kapatma olayi gibi olaylar bu degisken ile kontrol edilir

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) { //bu fonksiyon ile kullanicinin yaptigi olaylar alinir.
            if (e.type == SDL_QUIT) { //pencere kapatma olayi
                quit = true;
            } else if (e.type == SDL_KEYDOWN) { //klavyeden veri girisi
                switch (e.key.keysym.sym) { //basılan tusun sembolünü temsil eder
                    case SDLK_UP: //yukari
                        if (!carpismaKontrol(pacmanX, pacmanY - 1)) { //carpisma kontrolu yapılır carpisma yoksa yukari hareket edilir
                            pacmanY--;
                        }
                        break;
                    case SDLK_DOWN: //asagi
                        if (!carpismaKontrol(pacmanX, pacmanY + 1)) {  //carpisma kontrolu yapılır carpisma yoksa asagi hareket edilir
                            pacmanY++;
                        }
                        break;
                    case SDLK_LEFT:
                        if (!carpismaKontrol(pacmanX - 1, pacmanY)) {  //carpisma kontrolu yapılır carpisma sola yukari hareket edilir
                            pacmanX--;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (!carpismaKontrol(pacmanX + 1, pacmanY)) {  //carpisma kontrolu yapılır carpisma yoksa saga hareket edilir
                            pacmanX++;
                        }
                        break;
                }
            }
        }

        hayaletHareketi();  //fonksiyon cagirma

        if (hayaletCarpismaKontrolu()) { //carpisma kontrolu yapilir
            printf("Game Over!\n");
            quit = true;
        }

        if (yem[pacmanY][pacmanX]) { //eger pacman yemi yediyse + puan kazanir yemediyse degisiklik olmaz
            skor += 10;
            yem[pacmanY][pacmanX] = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Render alanının arkaplan rengi siyah yapilir
        SDL_RenderClear(renderer); //Render alanı temizlenir, önceki çizimler silinir.

        labirentCiz(renderer); // labirent ve karakterlerin çizimi için

        SDL_RenderPresent(renderer); //yapilan cizimlerin ekrana yansitilmasi icin
    }

    SDL_DestroyRenderer(renderer); //renderer kapatma
    SDL_DestroyWindow(window); //pencereyi kapatma
    SDL_Quit(); //SDL kütüphanesini kapatmak icin


    printf("Skor: %d\n", skor);

    return 0;
}
