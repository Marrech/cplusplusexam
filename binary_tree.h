// Furfaro Daniel 810933 d.furfaro1@campus.unimib.it

#pragma once

#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <iostream>
#include <cstddef>
#include <stdexcept>

/**
@file binary_tree.h
@brief Qui dichiaro la binary_tree class
**/

typedef unsigned int unsint;

template<typename T, typename F>
class binary_tree 
{
private:
    struct node 
    {
        T value; ///< Dato inserito nel nodo

        ///< Puntatori ai nodi padre, figlio sinistro e figlio destro
        node* father;
        node* left;
        node *right; 

        /**
            Costruttore di default
        */
        node() : value(nullptr), father(nullptr), left(nullptr), right(nullptr) {}

        /**
            Costruttore secondario che inizializza il nodo 
            @param init_v valore del dato
            @param init_f nodo padre
        */
        node(const T init_v, node* init_f) : value(init_v), father(init_f), left(nullptr), right(nullptr) {}

        /**
            Costruttore secondario che inizializza il nodo
            @param init_v valore del dato
            @param init_f nodo padre
            @param init_l nodo sinistro
            @param init_r nodo destro
        */
        node(const T init_v, node* init_f, node* init_l, node* init_r) : value(init_v), father(init_f), left(init_l), right(init_r) {}
    };

    node *root; ///< puntatore alla radice dell'albero
    F funct;

    /**
    Funzione per cercare un dato valore nell'albero

    @param search_v il valore da ricercare
    @return se il valore è presente, l'inidirizzo del nodo contenitore, altrimenti null
    **/
    node *search_node(const T & search_v) const
    {
        node *tmp_root = root;

        while(tmp_root != nullptr)
        {
            if (tmp_root->value == search_v)
            {
                return tmp_root;
            }
            if(funct(search_v, tmp_root->value) == 1)
            {
                tmp_root = tmp_root->right;
            }else
            {
                tmp_root = tmp_root->left;
            }
        }

        return nullptr;
    }
    
    /**
    Funzione che dal nodo passato crea una copia dell'albero

    @param new_root nodo da cui partirà il nuovo albero
    @return puntatore a root dell'albero copiato
    **/
    node *copy_tree_by_node(node * new_root) const
    {
        if (new_root == nullptr) { return nullptr; }
        else
        {
            return new node(static_cast<T>(new_root->value),
                nullptr,
                copy_tree(new_root->left),
                copy_tree(new_root->right));
        }
    }
   
    node *copy_tree(node *it_start) const 
    {
        node *n = copy_tree_by_node(it_start);

        return n;
    }


    /**
    Funzione per la pulizia dell'albero
    **/
    void clear_helper()
    {
        rec_node_del(root);
    }

    void rec_node_del(node* n) 
    {
        if (n == nullptr) return;
        rec_node_del(n->left);
        rec_node_del(n->right);
        n->father = nullptr;
        n->left = nullptr;
        n->right = nullptr;
        delete n;
    }

    /**
    Funzione per il calcolo della dimensione dell'albero

    @param tmp_node nodo da cui partire per la ricerca
    @return valore di dimensione dell'albero
    **/
    unsint size(const node *tmp_node)
    {
        if (tmp_node == nullptr)
        {
            return 0;
        }

        unsint size_l = size(tmp_node->left);

        unsint size_r = size(tmp_node->right);

        return 1 + size_l + size_r;
    }


public:
    /**
    Costruttore di default
    **/
    binary_tree() : root(nullptr) {}

    /**
    Costruttore che assegna alla radice il valore new_v

    @param new_v valore da assegnare alla radice
    **/
    explicit binary_tree(const T & new_v) : root(new node(new_v, nullptr)) {}

    /**
    Costruttore che crea una copia dell'albero passato come parametro

    @param tree albero da copiare
    **/
    binary_tree(const binary_tree &tree) 
    {
        try 
        {
            root = copy_tree(tree.root);
        } catch(...) 
        {
            clear_helper();
            throw;
        }
    }

    /**
    Distruttore
    **/
    ~binary_tree() 
    {
        clear_helper();
    }
    

    /**
    Funzione pubblica per il calolcolo della dimensione dell'albero (partendo dalla radice)

    @return valore di dimensione dell'albero
    **/
    unsint get_size() 
    {
        return size(root);
    }
    
    /**
    Funzione che cerca nell'albero l'elemento passato come parametro

    @param search_v valore da ricercare
    @return true se l'albero contiene search_v, altrimenti false
    **/
    bool search(const T & search_v) const
    {
        return search_node(search_v) != nullptr;
    }
    
    /**
    Funzione che passato un dato d dello stesso tipo del dato contenuto nell’albero, ritorna un nuovo albero.

    @param new_start valore del nodo da cui iniziare la copia
    @return albero copiato dal valore d in poi, altrimenti un albero vuoto
    **/
    binary_tree subtree(const T &new_start) const 
    {
        binary_tree<T, F> st;

        node *start = search_node(new_start);

        if (start != nullptr) 
        {
            st.root = copy_tree(start);
        }

        return st;
    }

    /**
    Operatore di assegnamento per la copia tra alberi

    @param other (l'oggetto da cui copiare)
    @return puntatore all'oggetto che lancia il metodo.
    **/
    binary_tree &operator=(const binary_tree &o_tree) 
    {
        if (this != &o_tree)
        {
            this->clear_helper();
            this->root = copy_tree(o_tree.root);
        }

        return *this;
    }

    /**
    Aggiunge nell'albero il valore passato come parametro

    @param new_v valore da inserire
    **/
    void add(const T &new_v) 
    {
        if(root == nullptr)
        {
            root = new node(new_v, nullptr);
            return;
        }
        find_position(root,new_v);
    }

    void find_position(node *check_node,const T& new_v)
    {
        if(funct(new_v, check_node->value) == 2)
        {
            throw std::runtime_error("<!Si sta cercando di inserire un valore già presente, eccezione gestista e dato non duplicato!>");
            return;
        }
        if(funct(new_v, check_node->value) == 1)
        {
            if(check_node->right != nullptr)
            {
                find_position(check_node->right, new_v);
            }
            else
            {
                check_node->right = new node(new_v, check_node);
            }
        }
        else
        {
            if (check_node->left != nullptr)
            {
                find_position(check_node->left, new_v);
            }
            else
            {
                check_node->left = new node(new_v, check_node);
            }
        }
    }

    /**
        Iteratore costante dell'albero

        @brief Iteratore costante dell'albero
    */
    class const_iterator 
    {
    private:
        friend class binary_tree;
        const node *it_start; 

        explicit const_iterator(const node *n) :it_start(n) {}

    public:
        typedef std::forward_iterator_tag   iterator_category;
        typedef node                        value_type;
        typedef ptrdiff_t                   difference_type;
        typedef const node                  *pointer;
        typedef const node                  &reference;

        const_iterator() : it_start(nullptr) {}

        const_iterator(const const_iterator &other) : it_start(other.it_start) {}
        

        const_iterator &operator=(const const_iterator &other) 
        {
            it_start = other.it_start;

            return *this;
        }

        ~const_iterator() {}

        // Ritorna il puntatore al dato riferito dall'iteratore
        pointer operator->() const 
        {
            return it_start;
        }

        // Ritorna il dato riferito dall'iteratore (dereferenziamento)
        reference operator*() const 
        {
            return *it_start;
        }
        
        // Operatore di iterazione post-incremento
        const_iterator operator++(int) 
        {
            const_iterator tmp(*this);
            if (it_start == nullptr) 
            {
                throw;
            }
            if (it_start->left != nullptr) 
            {
                it_start = it_start->left;
            }
            else if (it_start->right != nullptr) 
            {
                it_start = it_start->right;
            }
            else 
            {
                node* tmp_n = it_start->father;
                while (tmp_n != nullptr)
                {
                    if (tmp_n->right != nullptr && tmp_n->right != it_start) {
                        it_start = tmp_n->right;
                        return tmp;
                    }
                    else 
                    {
                        it_start = tmp_n;
                        tmp_n = it_start->father;
                    }
                }
                it_start = tmp_n;
            }

            return tmp;
        }

        const_iterator& operator++() 
        {
            if (it_start == nullptr) 
            {
                throw;
            }
            if (it_start->left != nullptr) 
            {
                it_start = it_start->left;
            }
            else if (it_start->right != nullptr) 
            {
                it_start = it_start->right;
            }
            else 
            {
                node* tmp_n = it_start->father;
                while (tmp_n != nullptr)
                {
                    if (tmp_n->right != nullptr && tmp_n->right != it_start)
                    {
                        it_start = tmp_n->right;
                        return*this;
                    }
                    else 
                    {
                        it_start = tmp_n;
                        tmp_n = it_start->father;
                    }
                }
                it_start = tmp_n;
            }
            return *this;
        }


        //Tentativo di operator++ post order
        /*const_iterator operator++(int) 
        {
            const_iterator tmp(*this);
            if (it_start == nullptr) throw;

            if(it_start->father == nullptr)
            {
                //std::cout << "Primo if" << std::endl;
                while (!(it_start->left == nullptr && it_start->right == nullptr))
                {
                    if (it_start->left != nullptr)
                    {
                        it_start = it_start->left;
                    }
                    else if (it_start->right != nullptr)
                    {
                        it_start = it_start->right;
                    }
                }
                //return *this;
            }
            else
            {
                T value_to;

                while (it_start->father != nullptr)
                {
                    value_to = it_start->value;
                    it_start = it_start->father;
                    //std::cout << "Valore padre:" << it_start->father->value << std::endl;
                    //std::cout << "Valore salvato: " << value_to << " Confronto: " << it_start->left->value << std::endl;
                    if (it_start->left != nullptr)
                    {
                        if (it_start->left->value == value_to)
                        {
                            if (it_start->right != nullptr)
                            {
                                it_start = it_start->right;
                                while (!(it_start->left == nullptr && it_start->right == nullptr))
                                {
                                    if (it_start->left != nullptr)
                                    {
                                        it_start = it_start->left;
                                    }
                                    else if (it_start->right != nullptr)
                                    {
                                        it_start = it_start->right;
                                    }
                                }
                                return tmp;
                            }
                            return tmp;
                        }
                    }
                    if (it_start->father == nullptr) 
                    {
                        it_start = it_start->father;
                    }
                    return tmp;
                }
            }

            return tmp;
        }*/

        // Uguaglianza
        bool operator==(const const_iterator &other) const 
        {
            return this->it_start == other.it_start;
        }

        // Diversita'
        bool operator!=(const const_iterator &other) const 
        {
            return this->it_start != other.it_start;
        }
    };

    // Ritorna l'iteratore all'inizio della sequenza dati
    const_iterator begin() const 
    {
        return const_iterator(root);
    }

    // Ritorna l'iteratore alla fine della sequenza dati
    const_iterator end() const 
    {
        return const_iterator(nullptr);
    }
};

/**
Ridefinizione dell'operatore di stream per la stampa del contenuto
dell'albero

@param os oggetto stream di output
@param tree albero da stampare

@return reference allo stream di output
**/
template <typename T, typename F>
std::ostream &operator<<(std::ostream &os, const binary_tree<T,F> &tree) 
{
    typename binary_tree<T, F>::const_iterator i, ie;

    i = tree.begin();
    ie = tree.end();

    while (i != ie) 
    {
        os << i.operator*().value << " ";
        i++;
    }

    os << std::endl;
    return os;
}

/**
Funzione che passato un predicato di tipo P restituisce in output gli elementi dell'albero
che lo soddisfano.

@param tree l'albero binario di ricerca
@param pred il predicato da soddisfare

@return reference allo stream di output
**/
template <typename T, typename F, typename P>
void printIF(const binary_tree<T,F> &tree, const P pred) 
{
    typename binary_tree<T, F>::const_iterator i, ie;

    i = tree.begin();
    ie = tree.end();

    while (i != ie) 
    {
        if (pred(i.operator*().value)) 
        {
            std::cout << i.operator*().value << " ";
        }
        i++;
    }

    std::cout << std::endl;
}

#endif
