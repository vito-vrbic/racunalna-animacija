# Računalna Animacija
Ovaj repozitorij koristi se za laboratorijske vježbe na kolegiju **Računalna Animacija**, na diplomskom studiju **Fakulteta elektronike i računarstva u Zagrebu**, akademske godine **25./26**.

## Laboratorijska vježba 1 - B-spline krivulje
Sve što je potrebno za **Laboratorijsku vježbu 1** nalazi se u folderu `/lab1`.

### Izgradnja i pokretanje
Za izgradnju `.exe` datoteke za testiranje potrebno je pokrenuti sljedeće `.bat` skripte:
- `/scripts/config.bat`
- `/scripts/build.bat`

Nakon izgradnje, `.exe` datoteka može se pokrenuti direktno ili preko `.bat` skripte:
- `/scripts/run.bat`

U oba slučaja potrebno je predati **dva argumenta**:
1. Putanju do `.obj` datoteke koja sadrži mrežu vrhova i trokutova.
2. Putanju do `.crv` datoteke koja sadrži poligon vrhova.

Primjeri `.crv` datoteka nalaze se u folderu `/assets`.

## Laboratorijska vježba 2 - Čestični sustavi
Sve što je potrebno za **Laboratorijsku vježbu 2** nalazi se u folderu `/lab2`.

### Izgradnja i pokretanje
Za izgradnju `.exe` datoteke za testiranje potrebno je pokrenuti sljedeće `.bat` skripte:
- `/scripts/config.bat`
- `/scripts/build.bat`

Nakon izgradnje, `.exe` datoteka može se pokrenuti direktno ili preko `.bat` skripte:

- `/scripts/run.bat`

## Projekt - Gerstnerovi valovi duboke vode
Sve što je potrebno za **Projekt** nalazi se u folderu `/projekt`.

### Izgradnja i pokretanje
Za izgradnju "igrice" korišten je Unity Editor 2022.3.62f2, a projekt sa svim datotekama za izgradnju nalazi se u folderu `/Waverider`.

Sama izvršna datoteka/build datoteka za "igricu" već je izgrađena i može se naći u folderu `/Build`

Kontrole za "igricu" su sljedeće:
- `W` za davanje brzine unaprijed,
- `S` za davanje brzine unatrag,
- `A` za skretanje ulijevo,
- `D` za skretanje udesno,
- `Mouse Right Click + Mouse Move` - pomicanje orbitalne kamere,
- `Mouse Scroll` - promjena zoom-a kamere,
- `Escape` - gašenje aplikacije.

U igrici se brod može kretati te se oko njega u radijusu stvaraju bačve koje isto kao i on plutaju na vodi. Pri koliziji bačve nestaju kao da ih brod kupi i time čisti ocean.
