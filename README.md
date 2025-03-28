<h3>Junkman The Concept. ??</h3> 

Kiedy? obudzi?em si? rano i id?c do porannej toalety zastanawia?em si?, do czego mo?na jeszcze wykorzysta? disassembler HDE (Hacker Disassembler Engine).
Czyli taki, kt車ry z regu?y s?u?y do obliczania d?ugo?ci instrukcji procesora. Tego typu disassemblery by?y historycznie wykorzystywane na przyk?ad w technice 
ukrywania punktu wej?cia (EPO). Jak wiadomo toaleta, jest najlepszym miejscem, w kt車rym cz?owiek wpada na genialne pomys?y i tak te? si? sta?o w tym przypadku.
Ot車? wymy?li?em, ?e wykorzystam HDE do tworzenia ?mieciowych instrukcji assmblera, a ?ci?lej m車wi?c nie b?d? ich tworzy? tylko kopiowa?.
Poniewa? b?d? to bloki instrukcji kopiowane z istniej?cego oprogramowania np. systemowego to nie b?dzie ich mo?na wykorzysta? do statycznej detekcji, poza tym b?d? 
to instrukcje trybu u?ytkownika, kt車re jak wiadomo pasuj? do program車w trybu u?ytkownika.

<h3>Rys historyczny.</h3>

Og車lnie silniki polimorficzne sk?adaj? si? z kilku g?車wnych komponent車w.<br/>
Assembler lub dissambler, kt車ry pozwala tworzy? lub modyfikowa? kod assmblera.<br/>
Permutator, kt車ry zmienia warianty instrukcji assemblera np. 
<pre>
mov eax, 1337
</pre>
na 
<pre>
mov eax, 1330, 
add eax, 7
</pre>
Komponent kt車ry zminia losowo wykorzystywane rejestry procesora i bloki kodu 〞 przy czym w?a?ciwie prawie ka?da pojedyncza instrukcja mo?e by? traktowana jako osobny blok podstawowy.<br/>
Opcjonalne elementy, kt車re np. wykrywaj? emulatory, debugery etc.<br/>
Komponent, kt車ry tworzy losowe instrukcje, aby zwi?kszy? losowo?? en/dekryptora.<br/>
Za?車?my, ?e tak wygl?da podstawowy dekryptor bez komponentu, kt車ry tworzy losowe instrukcje.

<pre>
[main_basic_block_a]
init: 
[basic_block_a]
(permutatoion) mov r, base_of_code
[basic_block_b]
(permutatoion) mov r, size_of_code
[basic_block_c]
(permutatoion) mov r, key
[main_basic_block_b]
loop_labler:
    (permutatoion) decrypt code
	(permutatoion) inc offset
	(permutatoion) loop
[main_basic_block_c]
redirect:
    (permutatoion) jmp 2_decrypted_code
</pre>
Tak natomiast wygl?da dekryptor z komponentem, kt車ry tworzy losowe instrukcje.

<pre>
[main_basic_block_a]
(permutatoion) jmp init
trash asm instructions
...
...
init: 
[basic_block_a]
(permutatoion) mov r, base_of_code
[basic_block_b]

(permutatoion) jmp over_a
trash asm instructions
...
...
over_a:

(permutatoion) mov r, size_of_code
[basic_block_c]
(permutatoion) jmp over_b
trash asm instructions
...
...
over_b:
(permutatoion) mov r, key
[main_basic_block_b]
loop_labler:
    (permutatoion) decrypt code
	(permutatoion) jmp over_c
trash asm instructions
...
...
over_c:
	(permutatoion) inc offset
	(permutatoion) loop
[main_basic_block_c]
redirect:
    (permutatoion) jmp over_d
trash asm instructions
...
...
over_d:
    (permutatoion) jmp 2_decrypted_code
</pre>

Og車lnie rzecz bior?c, mo?emy wstawi? do en/decryptora dowoln? ilo?? blok車w "?mieciowych instrukcji". <br/>
Historycznie te bloki instrukcji by?y tworzone oczywi?cie w j?zykach niskiego poziomu i kluczowe jest tu s?owo "tworzone" poniewa? tworzenie losowych instrukcji 
powodowa?o na przyk?ad, ?e w aplikacjach trybu u?ytkownika pojawia?y si? uprzywilejowane instrukcje trybu j?dra i og車lnie charakterystyka takich blok車w by?a kompletnie nie logiczna.
...
...
...
