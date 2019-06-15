# Systemy operacyjne - Projekt

## Tartak - Aplikacja wielowątkowa
**Cel projektu:** Napisać aplikację wielowątkową zgodną z poniższymi założeniami.

Miejsca do których mogą udać się drwale:
- Magazyn (`class Storage`)
- Jadalnia (`class EatingRoom`)
- Dom
- Drzewa (`class Tree`)

Typy wątków w projekcie:
- Drwale (`class Employee`)
- Magazyn (`class Storage`)
- Las (`class Forest`)
- Cykl dnia i nocy (`class DayNightCycle`)

Rodzaje zasobów:
- Drzewa (`class Tree`)
- Drewno oraz przerobione (przez magazyn) drewno
- Narzędzia (siekiery i piły)
- Jedzenie (poziom głodu drwali)

Wstępny opis:
Aplikacja ma symululować pracę drwali, którzy za dnia ścinają drzewa zanosząc drewno do magazynu, a w nocy wracają do domu. Drwale ścinając drzewa głodnieją, z tego powodu po pewnym czasie nie mają siły pracować i muszą iść coś zjeść do jadalni (wcześniej odkładając narzędzia do magazynu).

Główne założenia:
- Obrażenia drwala są tym mniejsze im większy poziom głodu.
- Gdy zapadnie noc drwale natychmiast kończą pracę wcześniej odkładając narzędzia do magazynu.
- Liczba siekier w magazynie jest nieskończona.
- Liczba pił w magazynie jest mniejsza niż drwali.
- Drwal z piłą i siekierą ścina ścina drzewa szybciej (zadaje większe obrażenia), niż z samą siekierą.
- Drwal nie może ścinać drzewa bez posiadania jakiegokolwiek narzędzia w ekwipunku.
- Drwal może zmieścić w plecaku maksymalnie dwie sztuki drewna.
- Drzewo może posiadać w sobie kilka sztuk drewna, które wypada przez zbijanie kolejnych segmentów "życia" drzewa.
- Gdy drwal zgłodnieje musi udać się do jadalni wcześniej odkładając wszystkie narzędzia do magazynu.
- W jadalni znajduje się ograniczona liczba miejsc. Gdy drawl postanowi pójść do jadalni uzupełnić głód musi poczekać na kolej.
- Drzewo może być ścinane przez dwóch drwali co znacznie przyspiesza ten proces.
- Gdy nastanie dzień drwale wychodzą z domu i zmierzają w kierunku magazynu