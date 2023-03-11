Úkolem je vytvořit program, který bude rozebírat sekvenci čísel.

Předpokládáme sekvenci nezáporných celých čísel (0, 1, 2, ...). Sekvence začíná číslem 0, jednotlivá čísla jsou zapsaná ve vzestupném pořadí bez vložených mezer a zbytečných nul. Sekvence bude začínat takto:

01234567891011121314151617181920...

Pro člověka je takový zápis obtížně čitelný, proto jej uvedeme ještě v obarvené podobě:

<img></img>

Úkolem programu je přečíst ze vstupu pozici (pořadové číslo od počátku) a určit, jaká číslice kterého čísla je na zadané pozici sekvence. Například:

    na pozici 0 je číslice 0 čísla 0,
    na pozici 15 je pak číslice 2 čísla 12,
    na pozici 2021 je číslice 1 čísla 710, ...

Aby byl program univerzálnější, dokáže zadaný problém řešit pro různé číselné soustavy. Například pro zápis čísel ve dvojkové soustavě by sekvence vypadala takto:

<img></img>

Pro dvojkovou soustavu např. bude na pozici 14 číslice 0 čísla 110.

Program bude mít na svém vstupu zadané jednotlivé problémy k vyřešení. Každý problém bude zadaný jako dvě celá desítková čísla. První z čísel udává pozici v sekvenci, druhé číslo pak základ soustavy, pro kterou problém řešíme. Pro každý takto zadaný problém program vypočte výsledek a zobrazí jej ve formátu podle ukázky (číslo s vyznačením vybrané číslice). Zadávání problémů skončí v okamžiku, kdy program dosáhne konce vstupu (je signalizován aktivní EOF).

Pokud je vstup neplatný, program to musí detekovat a zobrazit chybové hlášení. Chybové hlášení zobrazujte na standardní výstup (ne na chybový výstup). Za chybu považujte:

    pozice v sekvenci je neplatná (neplatný zápis čísla, záporná hodnota),
    základ číselné soustavy je neplatný (nečíselné zadání, mimo uzavřený interval < 2 ; 36 >).
