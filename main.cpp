// Furfaro Daniel 810933 d.furfaro1@campus.unimib.it


#include "binary_tree.h"
#include <iostream>
#include <cassert>

/**
@brief even

Funtore per il controllo dei numeri pari.
**/
struct even 
{
    bool operator()(const unsigned int &a) const 
    {
        return a%2 == 0;
    }
};

/**
@brief sort_by

Funtore per ordinamento decrescente.
**/
struct sort_by 
{
    unsigned int operator()(const unsigned int &a, const unsigned int &b) const 
    {
        if (a < b) { return 0; }
        else if (a > b) { return 1; }
        else { return 2; }
    }
};

std::string bool_to_string(bool b)
{
    if (b) { return "TRUE"; }
    else { return "FALSE"; }
}

void test_creazione_alberi() 
{
    std::cout << "Test dei costruttori:" << std::endl;
    std::cout << "(tree1)Creazione albero con valore 78 sulla radice:" << std::endl;
    binary_tree<unsint, sort_by> tree1(78);
    assert(tree1.search(78));
    std::cout << "(tree1)Aggiunta dei valori 64,19,22,107,136,8,24:" << std::endl;
    tree1.add(64);
    assert(tree1.search(64));
    tree1.add(19);
    assert(tree1.search(19));
    tree1.add(22);
    assert(tree1.search(22));
    tree1.add(107);
    assert(tree1.search(107));
    tree1.add(136);
    assert(tree1.search(136));
    tree1.add(8);
    assert(tree1.search(8));
    tree1.add(24);
    assert(tree1.search(24));
    std::cout << "(tree1)Test operatore di stampa albero (<<): " << std::endl;
    std::cout << tree1;
    std::cout << "(tree1)Dimensione albero: " << tree1.get_size() << std::endl;
    assert(tree1.get_size() == 8);
    std::cout << std::endl;

    std::cout << "Test copy constructor:" << std::endl;
    binary_tree<unsint, sort_by> tree2(tree1);
    std::cout << "(tree2)Copiato,dimensione nuovo albero: " << tree2.get_size() << std::endl;
    assert(tree2.get_size() == 8);
    std::cout << std::endl;

    std::cout << "Test operatore = :" << std::endl;
    std::cout << "(tree1)Aggiunta dei valori 215 e 38:" << std::endl;
    tree1.add(215);
    tree1.add(38);
    binary_tree<unsint, sort_by> tree3 = tree1;
    std::cout << "(tree3)Copiato,dimensione nuovo albero: " << tree3.get_size() << std::endl;
    assert(tree3.get_size() == 10);
    std::cout << std::endl;
}

void test_subtree()
{
    binary_tree<unsint, sort_by> tree6;
    std::cout << "Test subtree:" << std::endl;
    std::cout << "(tree6) Creazione di un nuovo albero vuoto e aggiunta dei valori 40,58,64,23,34,17,95,157,3,12: " << std::endl;
    tree6.add(40);
    tree6.add(58);
    tree6.add(64);
    tree6.add(23);
    tree6.add(34);
    tree6.add(17);
    tree6.add(95);
    tree6.add(157);
    tree6.add(3);
    tree6.add(12);
    std::cout << "(tree7) Creiamo un nuovo albero usando subtree(23)" << std::endl;
    binary_tree<unsint, sort_by> tree7 = tree6.subtree(23);
    std::cout << "(tree7) Albero risultante, dimensione: " << tree7.get_size() << std::endl;
    assert(tree7.search(23));
    assert(tree7.search(17));
    assert(tree7.search(34));
    assert(tree7.search(3));
    assert(tree7.search(12));
    std::cout << std::endl;
}

void test_search()
{
    std::cout << "Test della funzione Search:" << std::endl;
    binary_tree<unsint, sort_by> tree4;
    std::cout << "(tree4) Creo un nuovo albero e aggiungo i valori 18,4 e 36:" << std::endl;
    tree4.add(18);
    tree4.add(4);
    tree4.add(36);
    std::cout << "(tree4)Ricerca numeri 36 e 27:" << std::endl;
    assert(tree4.search(36) && !tree4.search(27));
    std::cout << "36 presente?: " << bool_to_string(tree4.search(36)) << "     27 presente?:" << bool_to_string(tree4.search(27)) << std::endl;
    std::cout << std::endl;
}

void test_iterator_and_print_if()
{
    binary_tree<unsint, sort_by> tree8;
    std::cout << "Test iteratori:" << std::endl;
    std::cout << "(tree8) Creazione nuovo albero con valori 40,58,64,59,23,34,17,95,157,3,12: " << std::endl;
    tree8.add(40);
    tree8.add(58);
    tree8.add(64);
    tree8.add(59);
    tree8.add(23);
    tree8.add(34);
    tree8.add(17);
    tree8.add(95);
    tree8.add(157);
    tree8.add(3);
    tree8.add(12);
    std::cout << "(tree8) Output albero tramite iteratori: " << std::endl;
    binary_tree<unsint, sort_by>::const_iterator it, ite;
    it = tree8.begin();
    ite = tree8.end();
    while (it != ite) {
        std::cout << it.operator*().value << " ";
        it++;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Test print if:" << std::endl;
    even e;
    std::cout << "(tree8) Output di tutti i valori positivi contenuti in tree8 :";
    printIF(tree8, e);
    std::cout << std::endl;
}

void test_try_add()
{
    binary_tree<unsint, sort_by> tree5;
    std::cout << "Test aggiunta di un valore gia presente:" << std::endl;
    std::cout << "(tree5)Creazione nuovo albero con valori 64,19,22,107,136:" << std::endl;
    tree5.add(64);
    tree5.add(19);
    tree5.add(22);
    tree5.add(107);
    tree5.add(136);
    assert(tree5.get_size() == 5);
    std::cout << "(tree5) Dimensione iniziale dell'albero: " << tree5.get_size() << std::endl;
    std::cout << "(tree5) Tentativo di aggiunta del valore 22..." << std::endl;
    try 
    {
        tree5.add(22);
    }
    catch (std::runtime_error) 
    { 
        std::cout << "<!Si sta cercando di inserire un valore gia presente, eccezione gestista e dato non duplicato!>" << std::endl;
    }
    std::cout << "(tree5) Dimensione albero dopo il tentativo di inserimento: " << tree5.get_size() << std::endl;
    assert(tree5.get_size() == 5);
    std::cout << std::endl;
}


int main()
{
    std::cout << "-----------------------------------------------" << std::endl;
    test_creazione_alberi();
    test_search();
    test_try_add();
    test_subtree();
    test_iterator_and_print_if();
    std::cout << "-----------------------------------------------" << std::endl;
}
