Úkolem je vytvořit program, který bude počítat množství látky potřebné pro vyrobení plachty.

Předpokládáme, že máme k dispozici látku pro výrobu plachet. Látka se dodává ve tvaru obdélníků v zadaném rozměru (výška, šířka). Dále dostaneme informaci o požadované velikosti plachty. Plachta je obdélníkového tvaru, zadaná je výška a šířka. Program určí, kolik kusů látky je potřeba sešít, aby vznikla plachta požadované velikosti.

Práce programu je trochu ztížena tím, že látky se v místě švu musí překrývat. Program tedy po načtení velikosti látky a plachty zjistí, zda bude potřeba látku sešívat. Pokud ano, zeptá se ještě na požadovanou velikost překryvu.

Při sešívání předpokládáme, že obdélníkové kusy látky skládáme vždy ve stejné orientaci v pravoúhlém rastru. Tedy obdélníky jsou buď všecnhy ponechané v zadané orientaci, nebo všechny otočené o 90 stupňů. Dále předpokládáme, že případné přebývající kusy látky nelze znovu využít.


<img></img>


Vstupem programu je zadání:

    velikosti látky - šířka a výška, desetinná čísla,
    velikosti požadované plachty - výška a šířka, desetinná čísla a
    velikost překryvu v místě švu (desetinné číslo). Program se na tuto informaci dotazuje jen pokud je potřeba látku sešívat.

Výstupem programu je nejmenší počet kusů látky potřebných pro výrobu plachty. Pro některé kombinace vstupů nelze plachtu vyrobit, program pak toto zobrazí (viz ukázka).

Pokud je vstup neplatný, program to musí detekovat a zobrazit chybové hlášení. Chybové hlášení zobrazujte na standardní výstup (ne na chybový výstup). Za chybu považujte:

    nečíselné zadání výšky, šířky nebo velikosti překryvu,
    výška nebo šířka je záporná nebo nulová,
    velikost překryvu je záporná (tedy připouští se nulový překryv),
    chybějící zadání výšky, šířky,
    chybějící zadání překryvu (v místě, kde je informace o překryvu nutná).
