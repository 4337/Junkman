<h3>Junkman The Concept. </h3> 

Kiedyœ obudzi³em siê rano i id¹c do porannej toalety zastanawia³em siê, do czego mo¿na jeszcze wykorzystaæ disassembler HDE (Hacker Disassembler Engine).
Czyli taki, który z regu³y s³u¿y do obliczania d³ugoœci instrukcji procesora. Tego typu disassemblery by³y historycznie wykorzystywane na przyk³ad w technice 
ukrywania punktu wejœcia (EPO). Jak wiadomo toaleta, jest najlepszym miejscem, w którym cz³owiek wpada na genialne pomys³y i tak te¿ siê sta³o w tym przypadku.
Otó¿ wymyœli³em, ¿e wykorzystam HDE do tworzenia œmieciowych instrukcji assmblera, a œciœlej mówi¹c nie bêdê ich tworzy³ tylko kopiowa³.
Poniewa¿ bêd¹ to bloki instrukcji kopiowane z istniej¹cego oprogramowania np. systemowego to nie bêdzie ich mo¿na wykorzystaæ do statycznej detekcji, poza tym bêd¹ 
to instrukcje trybu u¿ytkownika, które jak wiadomo pasuj¹ do programów trybu u¿ytkownika.

<h3>Rys historyczny.</h3>

Ogólnie silniki polimorficzne sk³adaj¹ siê z kilku g³ównych komponentów.<br/>
Assembler lub dissambler, który pozwala tworzyæ lub modyfikowaæ kod assmblera.<br/>
Permutator, który zmienia warianty instrukcji assemblera np. 
<pre>
mov eax, 1337
</pre>
na 
<pre>
mov eax, 1330, 
add eax, 7
</pre>
Komponent który zminia losowo wykorzystywane rejestry procesora i bloki kodu — przy czym w³aœciwie prawie ka¿da pojedyncza instrukcja mo¿e byæ traktowana jako osobny blok podstawowy.<br/>
Opcjonalne elementy, które np. wykrywaj¹ emulatory, debugery etc.<br/>
Komponent, który tworzy losowe instrukcje, aby zwiêkszyæ losowoœæ en/dekryptora.<br/>
Za³ó¿my, ¿e tak wygl¹da podstawowy dekryptor bez komponentu, który tworzy losowe instrukcje.

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
Tak natomiast wygl¹da dekryptor z komponentem, który tworzy losowe instrukcje.

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

Ogólnie rzecz bior¹c, mo¿emy wstawiæ do en/decryptora dowoln¹ iloœæ bloków "œmieciowych instrukcji". <br/>
Historycznie te bloki instrukcji by³y tworzone oczywiœcie w jêzykach niskiego poziomu i kluczowe jest tu s³owo "tworzone" poniewa¿ tworzenie losowych instrukcji 
powodowa³o na przyk³ad, ¿e w aplikacjach trybu u¿ytkownika pojawia³y siê uprzywilejowane instrukcje trybu j¹dra i ogólnie charakterystyka takich bloków by³a kompletnie nie logiczna.
...
...
...
