Pentru a realiza task-ul am folosit variabilele globale:
- `num_vars` - pentru a tine cont de numarul maxim de vairabile
- `num_clauses` - pentru a tine cont de numarul total de clauze
- `*vars` - vector pentru a tine cont de valoarea variabilelor(1, -1 , 0)
- `**clauses` - un array de vectori pentru a tine cont de clauze si de variabilele din ele

`vars` este initializat cu `num_vars + 1` ca sa nu mai trebuiasca sa ma plimb cu `- 1` cand vreau sa accesez un membru a lui `vars`, 
la fel si membrii pentru variabila ai lui `clauses`.

Parcurgerea printr-o clauza se face pana cand `clauses[i][j] == 0` deoarece este alocata mai multa memorie decat ar fi `"optim"` pentru
simplitate.

In functia de DPLL am folosit array-ul `backup[]` pentru a mentine integritatea lui `vars`.

Constanta `LINE_SIZE` am folosit-o pentru a initializa bufferul de citire pentru functia de read.

Functiile folosite:

## **read_file**
Functia citeste din fisier datele necesare.

Aceasta initializeaza `vars` si `clauses`. Sare peste liniile care incep cu `c`, citeste numarul de variabile si clauze. Dupa aceea citeste
fiecare linie pana la caracterul `0` si atribuie fiecarei clauze variabilele necesare.

## **write_answer**

Functia scrie in fisierul de output rezultatul lui DPLL.

Aceasta ruleaza algoritmul si pe baza rezultatului scrie outputul necesar.

## **is_satisfied**

Functia verifica daca toate clauzele din formula sunt satisfacute, adica cel putin un membru este adevarat.

## **unit_propagation**

Functia itereaza prin array-ul de clauze si cauta opertunitati de propagare. Aceasta:
- propaga varaibila doar cand mai exiata o singura variabila neasignata intr-o clauza
- daca gaseste o clauza cu toate variabilele asignate dar e nesatisfacuta returneaza `0`
- in cazul in care o modificare a fost facuta returneaza `1`.

## **clause_unsat**

Verifica daca o clauza este nesatisfacuta.

Itereaza prin clauza si daca clauza e satisfacuta sau varaibila(din `vars`) este nula returneaza `0`, altfel `1`.

## **DPLL**

Functia asta verifica daca expresia data poate fi sau nu satisfacuta.

Aceasta 
- propaga clauzele pana nu mai exista modificari
- verifica daca formula este satisfacuta 
- verifica daca exista clauze nesatisfacute
- daca formula nu este satisfacuta, face atribuiri de 1 sau -1 si face backtracking
- daca gaseste o solutie returneaza `1`, altfel `0`


