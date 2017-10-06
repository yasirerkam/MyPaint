// YASÝR ERKAM ÖZDEMÝR 2016/10

#include "stdafx.h"
#include <math.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <sstream>

struct Matris2B; 
struct Nokta3;
void RenkPaletiOlustur(ALLEGRO_BITMAP **bmpRenkler, ALLEGRO_COLOR *renkler, ALLEGRO_DISPLAY *display);
bool ButonaBasildi(Matris2B butonKonumu, ALLEGRO_MOUSE_STATE fareDurumu, ALLEGRO_BITMAP *bmp);
void KalemleCiz(ALLEGRO_MOUSE_STATE fareDurumu);
void ButonlariCizdir(Matris2B butonKonumu, ALLEGRO_BITMAP *bmp);
void ButonKonumlariAyarlaCiftSira(Matris2B *matris2B, int butonSayisi, int  butonXBaslangic, int butonYBaslangic, int kenarBoyu, int butonArasiFark);
void ButonKonumlariAyarlaTekSira(Matris2B *matris2B, int butonSayisi, int  butonXBaslangic, int butonYBaslangic, int kenarBoyu, int butonArasiFark);
void DuzCizgiCiz(Matris2B fareKonumlari);
void UcgenCiz(Nokta3 nokta3);
void DikdortgenCiz(Matris2B fareKonumlari);
void CemberCiz(Matris2B fareKonumlari);
void Sil(ALLEGRO_MOUSE_STATE fareDurumu);
void CizimRengiDegistir(bool *renkAktif, ALLEGRO_COLOR *renkler);
void CizimKalinligiDegistir(bool *cizimKalinligiAktif, int *cizimKalinliklari);
void YeniSayfaAc(ALLEGRO_DISPLAY *display);
void ResmiKaydet(ALLEGRO_BITMAP *bmpResim, ALLEGRO_DISPLAY *display);
void ResmiYukle(ALLEGRO_DISPLAY *display);
void SecimBolmesiniCizdir(Matris2B *konumButonlarCikisGurubu, Matris2B *konumButonlarCizim, Matris2B *konumButonlarRenkler, Matris2B *konumButonlarCizimKalinliklari,
	ALLEGRO_BITMAP **bmpRenkler, ALLEGRO_BITMAP **bmpCizimKalinliklari, ALLEGRO_BITMAP *butonCikis, ALLEGRO_BITMAP *butonYeniSayfa,
	ALLEGRO_BITMAP *butonKaydet, ALLEGRO_BITMAP *butonYukle, ALLEGRO_BITMAP *butonKalem, ALLEGRO_BITMAP *butonDuzCizgi, ALLEGRO_BITMAP *butonUcgen,
	ALLEGRO_BITMAP *butonDikdortgen, ALLEGRO_BITMAP *butonCember, ALLEGRO_BITMAP *butonSilgi, ALLEGRO_BITMAP *butonRenkleriDegistir);

const float FPS = 60;
int SCREEN_W;
int SCREEN_H;
int cizimKalinligi;
int secimBolgesi = 100;
ALLEGRO_COLOR cizimRengi;
const int renkSayisi = 8, cizimKalinligiSayisi = 6, butonCizimSayisi = 7, butonCikisGrubuSayisi = 4;

struct Matris2B {
	int x1, y1, x2, y2;
};
struct Nokta3 {
	int x1, y1, x2, y2, x3, y3;
};

int main(int argc, char **argv)
{
	// "init" fonksiyonlarý 
	if (!al_init()) {
		fprintf(stderr, "allegro yüklenemedi!\n");
		return -1;
	}
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "allegro primitives yüklenemedi!\n");
		return -1;
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "allegro image yüklenemedi!\n");
		return -1;
	}
	if (!al_init_ttf_addon()) {
		fprintf(stderr, "allegro ttf yüklenemedi!\n");
		return -1;
	}
	if (!al_init_font_addon()) {
		fprintf(stderr, "allegro font yüklenemedi!\n");
		return -1;
	}
	if (!al_init_native_dialog_addon()) {
		fprintf(stderr, "allegro dialog yüklenemedi!\n");
		return -1;
	}
	// "init" fonksiyonlarý bitiþ

	// deðiþkenler tanýmlama
	ALLEGRO_DISPLAY_MODE   disp_data;
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	SCREEN_W = disp_data.width;
	SCREEN_H = disp_data.height;
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	ALLEGRO_DISPLAY *display = NULL;

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *zamanlayici = NULL;
	ALLEGRO_BITMAP *butonCikis = al_load_bitmap("Sprites//ButonCikisGrubu//cikis.png");
	ALLEGRO_BITMAP *butonKaydet = al_load_bitmap("Sprites//ButonCikisGrubu//kaydet.png");
	ALLEGRO_BITMAP *butonYukle = al_load_bitmap("Sprites//ButonCikisGrubu//yukle.png");
	ALLEGRO_BITMAP *butonYeniSayfa = al_load_bitmap("Sprites//ButonCikisGrubu//yeniSayfa.jpg");
	ALLEGRO_BITMAP *butonKalem = al_load_bitmap("Sprites//ButonCizim//kalem.png");
	ALLEGRO_BITMAP *butonDuzCizgi = al_load_bitmap("Sprites//ButonCizim//duzCizgi.png");
	ALLEGRO_BITMAP *butonUcgen = al_load_bitmap("Sprites//ButonCizim//ucgen.png");
	ALLEGRO_BITMAP *butonDikdortgen = al_load_bitmap("Sprites//ButonCizim//dikdortgen.png");
	ALLEGRO_BITMAP *butonCember = al_load_bitmap("Sprites//ButonCizim//cember.png");
	ALLEGRO_BITMAP *butonSilgi = al_load_bitmap("Sprites//ButonCizim//silgi.png");
	ALLEGRO_BITMAP *butonRenkleriDegistir = al_load_bitmap("Sprites//ButonCizim//gokkusagi.png");
	ALLEGRO_BITMAP *bmpRenkler[renkSayisi];
	ALLEGRO_BITMAP *bmpCizimKalinliklari[cizimKalinligiSayisi];
	ALLEGRO_BITMAP *bmpResim = al_create_bitmap(SCREEN_W - secimBolgesi, SCREEN_H);
	ALLEGRO_COLOR renkler[renkSayisi];
	ALLEGRO_MOUSE_STATE fareDurumu;
	Matris2B konumButonlarCizim[butonCizimSayisi];//butonKalem, butonDuzCizgi, butonDikdortgen, butonUcgen, butonDaire, butonSilgi,butonRenkDegistir;
	Matris2B konumButonlarCikisGurubu[butonCikisGrubuSayisi];
	Matris2B konumButonlarRenkler[renkSayisi];
	Matris2B konumButonlarCizimKalinliklari[cizimKalinligiSayisi];
	int cizimKalinliklari[cizimKalinligiSayisi];
	float cizgi_dx = -4.0, cizgi_dy = 4.0;
	bool kapat, yeniSayfa, kaydet, yukle;
	kapat = yeniSayfa = kaydet = yukle = false;
	bool kalemAktif, duzCizgiCizAktif, ucgenCizAktif, dikdortgenCizAktif, daireCizAktif, silgiAktif;
	kalemAktif = duzCizgiCizAktif = ucgenCizAktif = dikdortgenCizAktif = daireCizAktif = silgiAktif = false;
	bool renkleriDegistir = false;
	bool renkAktif[renkSayisi];
	bool cizimKalinligiAktif[cizimKalinligiSayisi];
	// deðiþkenler tanýmlama bitiþ

	// "oluþturma" fonksiyonlarý
	zamanlayici = al_create_timer(1.0 / FPS);
	if (!zamanlayici) {
		fprintf(stderr, "zamanlayýcý oluþturulamadi!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		fprintf(stderr, "ekran oluþturulamadý!\n");
		al_destroy_timer(zamanlayici);
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "mause yüklenemedi!\n");
		al_destroy_display(display);
		al_destroy_timer(zamanlayici);
		al_destroy_bitmap(butonCikis);
		return -1;
	}

	// renkler atanýyor
	RenkPaletiOlustur(bmpRenkler, renkler, display);
	// renkler atanýyor bitiþ
	// renklerin ilk aktiflik durumu atanýyor
	renkAktif[0] = true;
	for (int i = 1; i < renkSayisi; ++i) {
		renkAktif[i] = false;
	}
	// renklerin ilk aktiflik durumu atanýyor bitiþ
	// cizgiKalinliklari spritelari atanýyor
	for (int i = 0; i < cizimKalinligiSayisi; ++i) {
		std::stringstream str;
		str << "Sprites//bmpCizimKalinligi//bmpCizimKalinligi" << i + 1 << ".png";
		bmpCizimKalinliklari[i] = al_load_bitmap(str.str().c_str());
		if (!bmpCizimKalinliklari[i]) {
			fprintf(stderr, "bmpCizimKalinligi yüklenemedi!\n");
			al_rest(3);
			return -1;
		}
	}
	// cizgiKalinliklari spritelari atanýyor bitiþ
	// cizgiKalinliklari atanýyor
	cizimKalinliklari[0] = 5;
	for (int i = 1; i < cizimKalinligiSayisi; ++i) {
		cizimKalinliklari[i] = cizimKalinliklari[i - 1] + 10;
		if (!bmpCizimKalinliklari[i]) {
			fprintf(stderr, "cizgiKalinliklari atanamadi!\n");
			al_rest(3);
			return -1;
		}
	}
	// cizgiKalinliklari atanýyor bitiþ
	// cizgiKalinliklari ilk aktiflik durumu atanýyor
	for (int i = 0; i < cizimKalinligiSayisi; ++i) {
		cizimKalinligiAktif[i] = false;
	}
	cizimKalinligiAktif[1] = true;
	// renklerin ilk aktiflik durumu atanýyor bitiþ
	// "oluþturma" fonksiyonlarý bitiþ

	// "yükleme" fonksiyonlarý
	ALLEGRO_FONT *colleged = al_load_ttf_font("Colleged.ttf", 40, NULL);
	ALLEGRO_FONT *bikinis = al_load_ttf_font("Bikinis.ttf", 150, NULL);
	if (!colleged || !bikinis) {
		fprintf(stderr, "font yüklenemedi!\n");
		al_destroy_display(display);
		al_destroy_timer(zamanlayici);
		al_rest(3);
		return -1;
	}
	// "yükleme" fonksiyonlarý bitiþ

	// Ýlk Ekran Görüntüleri
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(bikinis, al_map_rgb(255, 0, 0), SCREEN_W / 2-10, SCREEN_H / 2-50, ALLEGRO_ALIGN_CENTER, "MyPaint");
	al_flip_display();
	al_rest(1.25);
	al_clear_to_color(al_map_rgb(250, 200,20));
	al_draw_text(colleged, al_map_rgb(0, 120, 250), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTRE, "Yasir Erkam Ozdemir");
	al_flip_display();
	al_rest(2);

	// Ýlk Ekran Görüntüleri bitiþ

	// --butonlarýn konumlari ayarlanýyor
	ButonKonumlariAyarlaCiftSira(konumButonlarCikisGurubu, butonCikisGrubuSayisi, 17, 17, 30, 6);
	ButonKonumlariAyarlaTekSira(konumButonlarCizim, butonCizimSayisi, 25, 99, 50, 10);
	ButonKonumlariAyarlaCiftSira(konumButonlarRenkler, renkSayisi, 23, 525, 25, 5);
	ButonKonumlariAyarlaCiftSira(konumButonlarCizimKalinliklari, cizimKalinligiSayisi, 23, 650, 25, 5);
	// --butonlarýn konumlari ayarlanýyor bitiþ

	// çizim ekraný gösteriliyor
	al_clear_to_color(al_map_rgb(255, 255, 255));
	SecimBolmesiniCizdir(konumButonlarCikisGurubu, konumButonlarCizim, konumButonlarRenkler, konumButonlarCizimKalinliklari, bmpRenkler,
		bmpCizimKalinliklari, butonCikis, butonYeniSayfa, butonKaydet, butonYukle, butonKalem, butonDuzCizgi, butonUcgen, butonDikdortgen,
		butonCember, butonSilgi, butonRenkleriDegistir);
	// çizim ekraný gösteriliyor bitiþ

	// olay örgüsü komutlarý
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "olay kuyruðu oluþturulamadý!\n");
		al_destroy_bitmap(butonCikis);
		al_destroy_display(display);
		al_destroy_timer(zamanlayici);
		return -1;
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(zamanlayici));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	// olay örgüsü komutlarý bitiþ

	bool wDone = false;
	int ucgenKoseSayisi;

	// Döngüye giriliyor
	al_start_timer(zamanlayici);
	while (!wDone)
	{
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		al_get_mouse_state(&fareDurumu);

		if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			SecimBolmesiniCizdir(konumButonlarCikisGurubu, konumButonlarCizim, konumButonlarRenkler, konumButonlarCizimKalinliklari, bmpRenkler,
				bmpCizimKalinliklari, butonCikis, butonYeniSayfa, butonKaydet, butonYukle, butonKalem, butonDuzCizgi, butonUcgen, butonDikdortgen,
				butonCember, butonSilgi, butonRenkleriDegistir);
		}

		if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ) {

			if (ButonaBasildi(konumButonlarCikisGurubu[0], fareDurumu, butonCikis)) {
				wDone = true;
			}
			else if (ButonaBasildi(konumButonlarCikisGurubu[1], fareDurumu, butonYeniSayfa)) {
				YeniSayfaAc(display);
			}
			else if (ButonaBasildi(konumButonlarCikisGurubu[2], fareDurumu, butonKaydet)) {
				ResmiKaydet(bmpResim, display);
			}
			else if (ButonaBasildi(konumButonlarCikisGurubu[3], fareDurumu, butonYukle)) {
				ResmiYukle(display);
			}

			else if (ButonaBasildi(konumButonlarCizim[0], fareDurumu, butonKalem)) {
				if (kalemAktif)
					kalemAktif = false;
				else {
					kalemAktif = true;
					duzCizgiCizAktif = false;
					ucgenCizAktif = false;
					dikdortgenCizAktif = false;
					daireCizAktif = false;
					silgiAktif = false;
				}

			}
			else if (ButonaBasildi(konumButonlarCizim[1], fareDurumu, butonDuzCizgi)) {
				if (duzCizgiCizAktif)
					duzCizgiCizAktif = false;
				else {
					kalemAktif = false;
					duzCizgiCizAktif = true;
					ucgenCizAktif = false;
					dikdortgenCizAktif = false;
					daireCizAktif = false;
					silgiAktif = false;
				}
			}
			else if (ButonaBasildi(konumButonlarCizim[2], fareDurumu, butonUcgen)) {
				if (ucgenCizAktif)
					ucgenCizAktif = false;
				else {
					kalemAktif = false;
					duzCizgiCizAktif = false;
					ucgenCizAktif = true;
					dikdortgenCizAktif = false;
					daireCizAktif = false;
					silgiAktif = false;
					ucgenKoseSayisi = 0;
				}
			}
			else if (ButonaBasildi(konumButonlarCizim[3], fareDurumu, butonDikdortgen)) {
				if (dikdortgenCizAktif)
					dikdortgenCizAktif = false;
				else {
					kalemAktif = false;
					duzCizgiCizAktif = false;
					ucgenCizAktif = false;
					dikdortgenCizAktif = true;
					daireCizAktif = false;
					silgiAktif = false;
				}
			}
			else if (ButonaBasildi(konumButonlarCizim[4], fareDurumu, butonCember)) {
				if (daireCizAktif)
					daireCizAktif = false;
				else {
					kalemAktif = false;
					duzCizgiCizAktif = false;
					ucgenCizAktif = false;
					dikdortgenCizAktif = false;
					daireCizAktif = true;
					silgiAktif = false;
				}
			}
			else if (ButonaBasildi(konumButonlarCizim[5], fareDurumu, butonSilgi)) {
				if (silgiAktif)
					silgiAktif = false;
				else {
					kalemAktif = false;
					duzCizgiCizAktif = false;
					ucgenCizAktif = false;
					dikdortgenCizAktif = false;
					daireCizAktif = false;
					silgiAktif = true;
				}
			}
			else if (ButonaBasildi(konumButonlarCizim[6], fareDurumu, butonRenkleriDegistir)) {
				RenkPaletiOlustur(bmpRenkler, renkler, display);
			}

			for (int i = 0; i < renkSayisi; ++i)
			{
				if (ButonaBasildi(konumButonlarRenkler[i], fareDurumu, bmpRenkler[i]))
				{
					if (renkAktif[i])
						renkAktif[i] = false;
					else
					{
						for (int j = 0; j < renkSayisi; ++j) {
							if (i == j)
								renkAktif[j] = true;
							else
								renkAktif[j] = false;
						}
					}
				}
			}
			for (int i = 0; i < cizimKalinligiSayisi; ++i) {
				if (ButonaBasildi(konumButonlarCizimKalinliklari[i], fareDurumu, bmpCizimKalinliklari[i]))
				{
					if (cizimKalinligiAktif[i])
						cizimKalinligiAktif[i] = false;
					else
					{
						for (int j = 0; j < cizimKalinligiSayisi; ++j) {
							if (i == j)
								cizimKalinligiAktif[j] = true;
							else
								cizimKalinligiAktif[j] = false;
						}
					}
				}
			}

		}

		if (events.type == ALLEGRO_EVENT_TIMER) {
			CizimRengiDegistir(renkAktif, renkler);
			CizimKalinligiDegistir(cizimKalinligiAktif, cizimKalinliklari);
		}

		if (kalemAktif) {
			KalemleCiz(fareDurumu);
		}
		else if (duzCizgiCizAktif) {

			Matris2B fareKonumlari;
			if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (events.mouse.button & 1) {
					fareKonumlari.x1 = fareDurumu.x;
					fareKonumlari.y1 = fareDurumu.y;
					printf("baslangic : x : %d, y: %d\n", fareKonumlari.x1, fareKonumlari.y1);
				}
			}
			else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				if (events.mouse.button & 1) {
					fareKonumlari.x2 = fareDurumu.x;
					fareKonumlari.y2 = fareDurumu.y;
					printf("bitis :     x : %d, y: %d\n", fareKonumlari.x2, fareKonumlari.y2);
					DuzCizgiCiz(fareKonumlari);
				}
			}

		}
		else if (ucgenCizAktif) {
			Nokta3 nokta3;
			if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (events.mouse.button & 1) {
					if (ucgenKoseSayisi > 3) {
						ucgenKoseSayisi = 1;
					}
					switch (ucgenKoseSayisi) {
					case 0:
						ucgenKoseSayisi += 1;
						break;
					case 1:
						nokta3.x1 = fareDurumu.x;
						nokta3.y1 = fareDurumu.y;
						ucgenKoseSayisi += 1;
						printf("baslangic : x : %d, y: %d\n", nokta3.x1, nokta3.y1);
						break;
					case 2:
						nokta3.x2 = fareDurumu.x;
						nokta3.y2 = fareDurumu.y;
						printf("orta : x : %d, y: %d\n", nokta3.x2, nokta3.y2);
						ucgenKoseSayisi += 1;
						break;
					case 3:
						nokta3.x3 = fareDurumu.x;
						nokta3.y3 = fareDurumu.y;
						printf("bitis : x : %d, y: %d\n", nokta3.x3, nokta3.y3);
						UcgenCiz(nokta3); //    <<<<<<<<<<<<-----------
						ucgenKoseSayisi += 1;
						break;
					}
				}
			}

		}
		else if (dikdortgenCizAktif) {

			Matris2B fareKonumlari;
			if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (events.mouse.button & 1) {
					fareKonumlari.x1 = fareDurumu.x;
					fareKonumlari.y1 = fareDurumu.y;
					printf("baslangic : x : %d, y: %d\n", fareKonumlari.x1, fareKonumlari.y1);
				}
			}
			else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				if (events.mouse.button & 1) {
					fareKonumlari.x2 = fareDurumu.x;
					fareKonumlari.y2 = fareDurumu.y;
					printf("bitis :     x : %d, y: %d\n", fareKonumlari.x2, fareKonumlari.y2);
					DikdortgenCiz(fareKonumlari);
				}
			}

		}
		else if (daireCizAktif) {

			Matris2B fareKonumlari;
			if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (events.mouse.button & 1) {
					fareKonumlari.x1 = fareDurumu.x;
					fareKonumlari.y1 = fareDurumu.y;
					printf("baslangic : x : %d, y: %d\n", fareKonumlari.x1, fareKonumlari.y1);
				}
			}
			else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				if (events.mouse.button & 1) {
					fareKonumlari.x2 = fareDurumu.x;
					fareKonumlari.y2 = fareDurumu.y;
					printf("bitis :     x : %d, y: %d\n", fareKonumlari.x2, fareKonumlari.y2);
					CemberCiz(fareKonumlari);
					SecimBolmesiniCizdir(konumButonlarCikisGurubu, konumButonlarCizim, konumButonlarRenkler, konumButonlarCizimKalinliklari, bmpRenkler,
						bmpCizimKalinliklari, butonCikis, butonYeniSayfa, butonKaydet, butonYukle, butonKalem, butonDuzCizgi, butonUcgen, butonDikdortgen,
						butonCember, butonSilgi, butonRenkleriDegistir);
				}
			}

		}
		else if (silgiAktif) {
			Sil(fareDurumu);
		}

}


	al_destroy_font(colleged);
	al_destroy_bitmap(butonCikis);
	al_destroy_timer(zamanlayici);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
void RenkPaletiOlustur(ALLEGRO_BITMAP **bmpRenkler,ALLEGRO_COLOR *renkler,ALLEGRO_DISPLAY *display) 
{
	srand(time(NULL));
	for (int i = 0; i < renkSayisi; ++i) {
		bmpRenkler[i] = al_create_bitmap(25, 25);
		al_set_target_bitmap(bmpRenkler[i]);
		renkler[i] = al_map_rgb(rand() % 256, rand() % 256, rand() % 256);
		al_clear_to_color(renkler[i]);
	}
	al_set_target_bitmap(al_get_backbuffer(display));
}

void CizimRengiDegistir(bool *renkAktif,ALLEGRO_COLOR *renkler)
{
	for (int i = 0; i < renkSayisi; ++i) {
		if (renkAktif[i])
			cizimRengi = renkler[i];
	}
}
void CizimKalinligiDegistir(bool *cizimKalinligiAktif,int *cizimKalinliklari)
{
	for (int i = 0; i < cizimKalinligiSayisi; ++i) {
		if (cizimKalinligiAktif[i])
			cizimKalinligi = cizimKalinliklari[i];
	}
}

void KalemleCiz(ALLEGRO_MOUSE_STATE fareDurumu)
{
	if (fareDurumu.x > secimBolgesi) {
		if (fareDurumu.buttons & 1) {
			al_draw_filled_circle(fareDurumu.x, fareDurumu.y, cizimKalinligi, cizimRengi);
			al_flip_display();
		}
	}
}
void DuzCizgiCiz(Matris2B fareKonumlari)
{
	if (fareKonumlari.x1 > secimBolgesi && fareKonumlari.x2 > secimBolgesi) {
		al_draw_line(fareKonumlari.x1, fareKonumlari.y1, fareKonumlari.x2, fareKonumlari.y2, cizimRengi, cizimKalinligi);
		al_flip_display();
	}
}
void UcgenCiz(Nokta3 nokta3)
{
	if (nokta3.x1 > secimBolgesi && nokta3.x2 > secimBolgesi&& nokta3.x3 > secimBolgesi) {
		al_draw_triangle(nokta3.x1, nokta3.y1, nokta3.x2, nokta3.y2, nokta3.x3, nokta3.y3, cizimRengi, cizimKalinligi);
		al_flip_display();
	}
}
void DikdortgenCiz(Matris2B fareKonumlari)
{
	if (fareKonumlari.x1 > secimBolgesi && fareKonumlari.x2 > secimBolgesi) {
		al_draw_rectangle(fareKonumlari.x1, fareKonumlari.y1, fareKonumlari.x2, fareKonumlari.y2, cizimRengi, cizimKalinligi);
		al_flip_display();
	}
}
void CemberCiz(Matris2B fareKonumlari)
{
	if (fareKonumlari.x1 > secimBolgesi && fareKonumlari.x2 > secimBolgesi) {
		int yariCap = sqrt(pow((fareKonumlari.x2 - fareKonumlari.x1), 2) + pow((fareKonumlari.y2 - fareKonumlari.y1), 2));
		al_draw_circle(fareKonumlari.x1, fareKonumlari.y1, yariCap, cizimRengi, cizimKalinligi);
		al_flip_display();
	}
}
void Sil(ALLEGRO_MOUSE_STATE fareDurumu)
{
	if (fareDurumu.x > secimBolgesi) {
		if (fareDurumu.buttons & 1) {
			al_draw_filled_circle(fareDurumu.x, fareDurumu.y, cizimKalinligi, al_map_rgb(255, 255, 255));
			al_flip_display();
		}
	}
}

bool ButonaBasildi(Matris2B butonKonumu, ALLEGRO_MOUSE_STATE fareDurumu,ALLEGRO_BITMAP *bmp)
{
	if (fareDurumu.x > butonKonumu.x1 && fareDurumu.x < butonKonumu.x2 && fareDurumu.y < butonKonumu.y2 && fareDurumu.y>butonKonumu.y1 && (al_mouse_button_down(&fareDurumu, 1)))
	{
		al_draw_filled_rectangle(butonKonumu.x1, butonKonumu.y1, butonKonumu.x2, butonKonumu.y2, al_map_rgb(250, 200, 100)); //buton iç rengi
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1, butonKonumu.x1, butonKonumu.y2, al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1 - 1, butonKonumu.x2, butonKonumu.y1, al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y2, butonKonumu.x2, butonKonumu.y2 + 1, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(butonKonumu.x2, butonKonumu.y1, butonKonumu.x2 + 1, butonKonumu.y2, al_map_rgb(255, 255, 255));
		al_draw_scaled_bitmap(bmp, 0, 0, al_get_bitmap_width(bmp), al_get_bitmap_height(bmp), butonKonumu.x1 + 5, butonKonumu.y1 + 5, butonKonumu.x2 - butonKonumu.x1 - 10, butonKonumu.y2 - butonKonumu.y1 - 10, NULL);
		al_flip_display();
		return true;
	}
	else 
	{
		/*al_draw_filled_rectangle(butonKonumu.x1 - 2, butonKonumu.y1 - 2, butonKonumu.x2 + 2, butonKonumu.y2 + 2, al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(butonKonumu.x1, butonKonumu.y1, butonKonumu.x2, butonKonumu.y2, al_map_rgb(255, 255, 255)); //buton iç rengi
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1, butonKonumu.x1, butonKonumu.y2, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1 - 1, butonKonumu.x2, butonKonumu.y1, al_map_rgb(255, 255, 255));
		al_draw_scaled_bitmap(bmp, 0, 0, al_get_bitmap_width(bmp), al_get_bitmap_height(bmp), butonKonumu.x1 + 2, butonKonumu.y1 + 2, butonKonumu.x2 - butonKonumu.x1 - 4, butonKonumu.y2 - butonKonumu.y1 - 4, NULL);
		al_flip_display();*/
		return false;
	}
}
void SecimBolmesiniCizdir(Matris2B *konumButonlarCikisGurubu, Matris2B *konumButonlarCizim, Matris2B *konumButonlarRenkler, Matris2B *konumButonlarCizimKalinliklari,
	ALLEGRO_BITMAP **bmpRenkler, ALLEGRO_BITMAP **bmpCizimKalinliklari, ALLEGRO_BITMAP *butonCikis, ALLEGRO_BITMAP *butonYeniSayfa,
	ALLEGRO_BITMAP *butonKaydet, ALLEGRO_BITMAP *butonYukle, ALLEGRO_BITMAP *butonKalem, ALLEGRO_BITMAP *butonDuzCizgi, ALLEGRO_BITMAP *butonUcgen,
	ALLEGRO_BITMAP *butonDikdortgen, ALLEGRO_BITMAP *butonCember, ALLEGRO_BITMAP *butonSilgi, ALLEGRO_BITMAP *butonRenkleriDegistir)
{
	al_draw_scaled_bitmap(bmpRenkler[0], 0, 0, al_get_bitmap_width(bmpRenkler[0]), al_get_bitmap_height(bmpRenkler[0]), 0, 0, secimBolgesi, SCREEN_H, NULL);

	//butonlar cizdiriliyor
	ButonlariCizdir(konumButonlarCikisGurubu[0], butonCikis);
	ButonlariCizdir(konumButonlarCikisGurubu[1], butonYeniSayfa);
	ButonlariCizdir(konumButonlarCikisGurubu[2], butonKaydet);
	ButonlariCizdir(konumButonlarCikisGurubu[3], butonYukle);
	ButonlariCizdir(konumButonlarCizim[0], butonKalem);
	ButonlariCizdir(konumButonlarCizim[1], butonDuzCizgi);
	ButonlariCizdir(konumButonlarCizim[2], butonUcgen);
	ButonlariCizdir(konumButonlarCizim[3], butonDikdortgen);
	ButonlariCizdir(konumButonlarCizim[4], butonCember);
	ButonlariCizdir(konumButonlarCizim[5], butonSilgi);
	ButonlariCizdir(konumButonlarCizim[6], butonRenkleriDegistir);
	for (int i = 0; i < renkSayisi; ++i) {
		ButonlariCizdir(konumButonlarRenkler[i], bmpRenkler[i]);
	}
	for (int i = 0; i < cizimKalinligiSayisi; ++i) {
		ButonlariCizdir(konumButonlarCizimKalinliklari[i], bmpCizimKalinliklari[i]);
	}
	al_flip_display();
}
void ButonlariCizdir(Matris2B butonKonumu, ALLEGRO_BITMAP *bmp)
{
	al_draw_filled_rectangle(butonKonumu.x1 - 2, butonKonumu.y1 - 2, butonKonumu.x2 + 2, butonKonumu.y2 + 2, al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(butonKonumu.x1, butonKonumu.y1, butonKonumu.x2, butonKonumu.y2, al_map_rgb(255, 255, 255)); //buton iç rengi
	al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1, butonKonumu.x1, butonKonumu.y2, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1 - 1, butonKonumu.x2, butonKonumu.y1, al_map_rgb(255, 255, 255));
	al_draw_scaled_bitmap(bmp, 0, 0, al_get_bitmap_width(bmp), al_get_bitmap_height(bmp), butonKonumu.x1 + 2, butonKonumu.y1 + 2, butonKonumu.x2 - butonKonumu.x1 - 4, butonKonumu.y2 - butonKonumu.y1 - 4, NULL);
	//al_flip_display();
}

void ButonKonumlariAyarlaCiftSira(Matris2B *matris2B, int butonSayisi,int  butonXBaslangic,int butonYBaslangic,int kenarBoyu, int butonArasiFark)
{
	matris2B[0].y1 = butonYBaslangic;
	matris2B[0].y2 = butonYBaslangic + kenarBoyu;
	for (int i = 0; i < butonSayisi; i += 2) {
		matris2B[i].x1 = butonXBaslangic;
		matris2B[i].x2 = butonXBaslangic + kenarBoyu;
	}
	for (int i = 2; i < butonSayisi; i += 2) {
		matris2B[i].y1 = matris2B[i - 2].y1 + kenarBoyu + butonArasiFark;
		matris2B[i].y2 = matris2B[i - 2].y2 + kenarBoyu + butonArasiFark;
	}
	matris2B[1].y1 = butonYBaslangic;
	matris2B[1].y2 = butonYBaslangic + kenarBoyu;
	for (int i = 1; i < butonSayisi; i += 2) {
		matris2B[i].x1 = butonXBaslangic + kenarBoyu + butonArasiFark;
		matris2B[i].x2 = butonXBaslangic + 2 * kenarBoyu + butonArasiFark;
	}
	for (int i = 3; i < butonSayisi; i += 2) {
		matris2B[i].y1 = matris2B[i - 2].y1 + kenarBoyu + butonArasiFark;
		matris2B[i].y2 = matris2B[i - 2].y2 + kenarBoyu + butonArasiFark;
	}
}
void ButonKonumlariAyarlaTekSira(Matris2B *matris2B, int butonSayisi, int  butonXBaslangic, int butonYBaslangic, int kenarBoyu, int butonArasiFark)
{
	matris2B[0].y1 = butonYBaslangic;
	matris2B[0].y2 = butonYBaslangic + kenarBoyu;
	for (int i = 0; i < butonSayisi; ++i) {
		matris2B[i].x1 = butonXBaslangic;
		matris2B[i].x2 = butonXBaslangic + kenarBoyu;
	}
	for (int i = 1; i < butonSayisi; ++i) {
		matris2B[i].y1 = matris2B[i - 1].y1 + kenarBoyu + butonArasiFark;
		matris2B[i].y2 = matris2B[i - 1].y2 + kenarBoyu + butonArasiFark;
	}
}

void YeniSayfaAc(ALLEGRO_DISPLAY *display)
{
	ALLEGRO_BITMAP *bmpYeniSayfa = al_create_bitmap(SCREEN_W-secimBolgesi, SCREEN_H);
	if (bmpYeniSayfa) {
		al_set_target_bitmap(bmpYeniSayfa);
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_set_target_bitmap(al_get_backbuffer(display));
		al_draw_bitmap(bmpYeniSayfa, secimBolgesi, 0, 0);
	}
	else
	{
		al_show_native_message_box(display, "Hata!", "Yukleme Hatasi!", "Yeni sayfa yüklenemedi!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}
}
void ResmiKaydet(ALLEGRO_BITMAP *bmpResim,ALLEGRO_DISPLAY *display)
{
	al_set_target_bitmap(bmpResim);
	al_draw_bitmap_region(al_get_backbuffer(display), secimBolgesi, 0, al_get_bitmap_width(bmpResim), al_get_bitmap_height(bmpResim), 0, 0, 0);
	if (!al_save_bitmap("Kaydet//resim.png", bmpResim)) {
		al_show_native_message_box(display, "Hata!", "Kaydetme Hatasi!", "Resim Kaydedilemedi!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}
	al_set_target_bitmap(al_get_backbuffer(display));
}
void ResmiYukle (ALLEGRO_DISPLAY *display)
{
	ALLEGRO_BITMAP *bmpResim = al_load_bitmap("Kaydet//resim.png");
	if (bmpResim) {
		al_draw_bitmap(bmpResim, secimBolgesi, 0, 0);
	}
	else
	{
		al_show_native_message_box(display, "Hata!", "Yukleme Hatasi!", "Kayitli Resim Bulunamadi!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}
}