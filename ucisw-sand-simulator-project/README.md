# Układy cyfrowe i systemy wbudowane - Projekt
## Symulator ziaren piasku
**Cel projektu:** Stworzyć program symulujący zachowanie ziaren piasku (w tym opadanie i rozsypywanie się na boki). Użytkownik będzie miał możliwość sterowania kursorem na ekranie, wkazując tym samym miejsce do stworzenia lub usunięcia ziaren piasku (poprzez przytrzymanie odpowiednich przycisków).

**Założenia:** Program musi być napisany w języku VHDL i uruchamiać się na płytce Spartan 3.

**Moduły w projekcie:**
- `PS2_Kbd` - odczytywanie stanu klawiatury
Link do modułu: http://www.zsk.ict.pwr.wroc.pl/zsk_ftp/fpga/#_Toc479592711
- `VGADriver` (`VGADriver.vhd`) - wyświetlanie pikseli na ekranie
- `PowderSimulator` (`PowderSimulator.vhd`) - tworzenie i symulowanie fizyki ziaren piasku

## Efekt końcowy:
![](final_effect.gif)