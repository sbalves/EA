#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct piece
{
    int used = 0;
    int position;
    int rot = 0;
    int rots[4][4];
    vector<piece *> compatibles;
};

void rotate(piece *p);
int is_match(piece &t1, piece &p2, bool esquerdacima, int rt);
int combine_pieces(piece p1, piece p2);
void preprocessing();
int solve_puzzle_v2(vector<piece *> comp, int j, int i);
vector<piece *> intersect(vector<piece *> comp1, vector<piece *> comp2);
void show_table();

int num_columns, num_pieces, num_lines, num_puzzles;
vector<piece> pool;
vector<piece *> table;

void rotate(piece *p)
{
    for (int i = 0; i < 4; i++)
    {
        (*p).rots[1][i] = (*p).rots[0][(i + 1) % 4];
        (*p).rots[2][i] = (*p).rots[0][(i + 2) % 4];
        (*p).rots[3][i] = (*p).rots[0][(i + 3) % 4];
    }
}

void print_piece(piece p)
{
    for (int i = 0; i < 4; i++)
    {
        cout << p.rots[p.rot][i] << " ";
    }
    cout << endl;
}

void preprocessing()
{
    // comparar as da pool
    for (int i = 0; i < num_pieces; i++)
    {
        cout << "peca1: ";
        print_piece(pool[i]);
        for (int j = i + 1; j < num_pieces; j++)
        {
            if (combine_pieces(pool[i], pool[j]))
            {
                cout << "peca2: ";
                print_piece(pool[j]);
                pool[i].compatibles.push_back(&pool[j]);
                pool[j].compatibles.push_back(&pool[i]);
            }
            pool[i].rot = 0;
            pool[j].rot = 0;
        }
        cout << endl;
    }
}

int combine_pieces(piece p1, piece p2)
{
    for (int i = 0; i < 4; i++)
    {
        if (is_match(p1, p2, 0, i))
        {
            return 1;
        }
    }
    return 0;
}

int is_match(piece &t1, piece &p2, bool esquerdacima, int rt)
{
    // se as duas cores de t1 n tiverem na peca p2 n continues
    for (int i = p2.rot; i < 4; i++)
    {
        if (!esquerdacima)
        {
            /*
            cout << t1.rots[t1.rot][1] << "diferente" << p2.rots[i][0] << endl;
            cout << t1.rots[t1.rot][2] << "diferente" << p2.rots[i][3] << endl;
            cout << endl;
            */
            if (t1.rots[rt][1] == p2.rots[i][0] && t1.rots[rt][2] == p2.rots[i][3])
            {
                p2.rot = i;
                return 1;
            }
        }
        else
        {
            /*
            cout << t1.rots[t1.rot][3] << "diferente" << p2.rots[i][0] << endl;
            cout << t1.rots[t1.rot][2] << "diferente" << p2.rots[i][1] << endl;
            cout << endl;
            */
            if (t1.rots[rt][3] == p2.rots[i][0] && t1.rots[rt][2] == p2.rots[i][1])
            {
                p2.rot = i;
                return 1;
            }
        }
    }

    return 0;
}

vector<piece *> intersect(vector<piece *> comp1, vector<piece *> comp2)
{
    vector<piece *> inter_tmp;
    for (int i = 0; i < (int)comp1.size(); i++)
    {
        for (int j = 0; j < (int)comp2.size(); j++)
        {
            if (comp1[i] == comp2[j])
            {
                inter_tmp.push_back(comp2[j]);
            }
        }
    }
    return inter_tmp;
}

void show_table()
{
    for (int i = 0; i < num_lines; i++)
    {
        for (int k = 0; k < 2; k++)
        {

            for (int j = 0; j < num_columns; j++)
            {
                cout << table[i * num_columns + j]->rots[table[i * num_columns + j]->rot][0 + 3 * k] << " " << table[i * num_columns + j]->rots[table[i * num_columns + j]->rot][1 + k];
                if (j < num_columns - 1)
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
        if (i != num_lines - 1)
            cout << endl;
    }
}

int solve_puzzle_v2(vector<piece *> comp, int j, int i)
{

    if (j / num_columns == 0)
    { // linha = 0
        if (j == 0 || is_match(*table[j - 1], *comp[i], 0, table[j - 1]->rot))
        {
            // cout << "match! j: "<<  j << endl;
            if (j != 0)
            {

                table.push_back(comp[i]);
                comp[i]->used = 1;
            }
            if (j == num_pieces - 1)
            {
                return 1;
            }
            if (j % num_columns == num_columns - 1) // ultima coluna
            {
                // cout << "macaCo2" << endl;                                               // coluna = ultima
                for (int k = 0; k < (int)table[j - num_columns + 1]->compatibles.size(); k++) // os comps da primeira peca da linha
                {
                    if (table[j - num_columns + 1]->compatibles[k]->used == 0)
                    {
                        while (table[j - num_columns + 1]->compatibles[k]->rot < 4)
                        {
                            /*
                            cout << "rot1: "<< table[j - num_columns + 1]->compatibles[k]->rot << endl;

                            for(int p = 0; p < 4; p++){
                                cout << table[j - num_columns + 1]->compatibles[k]->rots[table[j - num_columns + 1]->compatibles[k]->rot][p]  << " ";
                            }

                            cout << endl;

                            */
                            if (solve_puzzle_v2(table[j - num_columns + 1]->compatibles, j + 1, k))
                            {
                                return 1;
                            }
                            // cout << "rot2: "<< table[j - num_columns + 1]->compatibles[k]->rot << endl;
                            table[j - num_columns + 1]->compatibles[k]->rot++;
                        }
                        table[j - num_columns + 1]->compatibles[k]->used = 0;
                        table[j - num_columns + 1]->compatibles[k]->rot = 0;
                        if ((int)table.size() > j + 1)
                            table.pop_back();
                    }
                }
                return 0;
            }
            else
            {
                // mcout << "macaco1" << endl;
                for (int k = 0; k < (int)table[j]->compatibles.size(); k++) // os comps da  ultima peca do tab
                {
                    if (table[j]->compatibles[k]->used == 0)
                    {
                        while (table[j]->compatibles[k]->rot < 4)
                        {
                            /*
                            cout << "macaco1: " << endl;

                            for(int p = 0; p < 4; p++){
                                cout << table[j]->compatibles[k]->rots[table[j]->compatibles[k]->rot][p]  << " ";
                            }

                            cout << endl;
                            */
                            if (solve_puzzle_v2(table[j]->compatibles, j + 1, k))
                            {
                                return 1;
                            }
                            table[j]->compatibles[k]->rot++;
                        }
                        table[j]->compatibles[k]->used = 0;
                        table[j]->compatibles[k]->rot = 0;
                        if ((int)table.size() > j + 1)
                            table.pop_back();
                    }
                }
                return 0;
            }
        }
        comp[i]->rot = 3;
        return 0;
    }
    else if (j % num_columns == 0)
    { // coluna = 0
        if (is_match(*table[j - num_columns], *comp[i], 1, table[j - num_columns]->rot) || j == 0)
        {
            if (j != 0)
            {

                table.push_back(comp[i]);
                comp[i]->used = 1;
            }
            if (j == num_pieces - 1)
            {
                return 1;
            }
            if (j % num_columns != num_columns - 1) // n Ã© a ultima
            {
                /*
                cout << "Comp1: " << endl;
                for (int k = 0; k < table[j]->compatibles.size(); k++)
                {
                    for (int l = 0; l < 4; l++)
                    {
                        cout << table[j]->compatibles[k]->rots[table[j]->compatibles[k]->rot][l] << " ";
                    }
                    cout << endl;
                }
                cout << endl;

                cout << "Comp2: " << endl;
                for (int k = 0; k < table[j - num_columns + 1]->compatibles.size(); k++)
                {
                    for (int l = 0; l < 4; l++)
                    {
                        cout << table[j - num_columns + 1]->compatibles[k]->rots[table[j - num_columns + 1]->compatibles[k]->rot][l] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
                */
                vector<piece *> compi = intersect(table[j]->compatibles, table[j - num_columns + 1]->compatibles);
                /*
                for (int k = 0; k < compi.size(); k++)
                {
                    for (int l = 0; l < 4; l++)
                    {
                        cout << compi[k]->rots[compi[k]->rot][l] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
                */
                // cout << compi.size() << endl;
                for (int k = 0; k < (int)compi.size(); k++)
                {
                    if (compi[k]->used == 0)
                    {
                        // cout << "unused " << k << endl;
                        while (compi[k]->rot < 4)
                        {
                            if (solve_puzzle_v2(compi, j + 1, k))
                            {
                                return 1;
                            }
                            compi[k]->rot++;
                        }
                        compi[k]->used = 0;
                        compi[k]->rot = 0;
                        if ((int)table.size() > j + 1)
                            table.pop_back();
                    }
                }
                return 0;
            }
            else
            {
                for (int k = 0; k < (int)table[j]->compatibles.size(); k++) // os comps da  ultima peca do tab
                {
                    if (table[j]->compatibles[k]->used == 0)
                    {
                        while (table[j]->compatibles[k]->rot < 4)
                        {
                            if (solve_puzzle_v2(table[j]->compatibles, j + 1, k))
                            {
                                return 1;
                            }
                            table[j]->compatibles[k]->rot++;
                        }
                        table[j]->compatibles[k]->used = 0;
                        table[j]->compatibles[k]->rot = 0;
                        if ((int)table.size() > j + 1)
                            table.pop_back();
                    }
                }
                return 0;
            }
            return 0;
        }
        comp[i]->rot = 3;
        return 0;
    }
    else
    { // resto
        if (is_match(*table[j - 1], *comp[i], 0, table[j - 1]->rot) && is_match(*table[j - num_columns], *comp[i], 1, table[j - num_columns]->rot))
        {

            table.push_back(comp[i]);
            comp[i]->used = 1;
            if (j == num_pieces - 1)
            {

                return 1;
            }
            if (j % num_columns != num_columns - 1)
            {
                vector<piece *> compi = intersect(table[j]->compatibles, table[j - num_columns + 1]->compatibles);
                for (int k = 0; k < (int)compi.size(); k++)
                {
                    if (compi[k]->used == 0)
                    {
                        while (compi[k]->rot < 4)
                        {
                            if (solve_puzzle_v2(compi, j + 1, k))
                            {
                                return 1;
                            }
                            compi[k]->rot++;
                        }
                        compi[k]->used = 0;
                        compi[k]->rot = 0;
                        if ((int)table.size() > j + 1)
                            table.pop_back();
                    }
                }
                return 0;
            }
            else
            {
                for (int k = 0; k < (int)table[j - num_columns + 1]->compatibles.size(); k++) // os comps da primeira peca da linha
                {
                    if (table[j - num_columns + 1]->compatibles[k]->used == 0)
                    {
                        while (table[j - num_columns + 1]->compatibles[k]->rot < 4)
                        {
                            if (solve_puzzle_v2(table[j - num_columns + 1]->compatibles, j + 1, k))
                            {
                                return 1;
                            }
                            table[j - num_columns + 1]->compatibles[k]->rot++;
                        }
                        table[j - num_columns + 1]->compatibles[k]->used = 0;
                        table[j - num_columns + 1]->compatibles[k]->rot = 0;
                        if ((int)table.size() > j + 1)
                            table.pop_back();
                    }
                }
                return 0;
            }
        }
        comp[i]->rot = 3;
        return 0;
    }
    return 0;
}

int main()
{
    cin >> num_puzzles;
    for (int j = 0; j < num_puzzles; j++)
    {
        do
        {
            // cout << "Insira variaveis validas:" << endl;
            cin >> num_pieces >> num_lines >> num_columns;
            // cout << "numpieces: " << num_pieces << endl;
            // cout << "numlines: " << num_lines << endl;
            // cout << "numcolumns: " << num_columns << endl;
        } while (num_lines * num_columns != num_pieces && num_columns);

        struct piece p;
        int colors[999] = {0}, color_evens = 0;

        for (int u = 0; u < num_pieces; u++)
        {
            for (int i = 0; i < 4; i++)
            {
                cin >> p.rots[0][i];
                colors[p.rots[0][i]]++;
            }
            rotate(&p);

            pool.push_back(p);

            // else
        }

        preprocessing();
        table.push_back(&pool[0]);
        pool[0].used = 1;
        /*
        cout << "pool0" << endl;
        for (int j = 0; j < 4; j++)
        {
            cout << pool[0].compatibles[0]->rots[0][j] << " ";
        }
        cout << endl;
        cout << "table0" << endl;
        cout << table[0]->compatibles.size() << endl;
        for (int j = 0; j < 4; j++)
        {
            cout << table[0]->compatibles[0]->rots[0][j] << " ";
        }
        cout << endl;
        */
        /*
        pool[0].compatibles[1]->rots[pool[0].compatibles[1]->rot][0] = 0;
        cout << "comp com primeira: "<< endl;
        for (int i = 0; i < pool[0].compatibles.size(); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                cout << pool[0].compatibles[i]->rots[pool[0].compatibles[i]->rot][j] << " ";
            }
            cout << endl;
        }
        cout << "comp com ultima: "<< endl;
        for (int i = 0; i < pool[3].compatibles.size(); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                cout << pool[3].compatibles[i]->rots[pool[3].compatibles[i]->rot][j] << " ";
            }
            cout << endl;
        }
        */
        for (int i = 0; i < 999; i++)
        {
            if (colors[i] % 2 != 0)
            {
                color_evens++;
                // cout << colors[i] << endl;
            }
        }

        if (color_evens > 4)
        {
            cout << "impossible puzzle!" << endl;
        }
        else
        {
            if (!solve_puzzle_v2(table[0]->compatibles, 0, 0))
            {
                cout << "impossible puzzle!" << endl;
            }
            else
            {
                show_table();
            }
        }

        pool.clear();
        table.clear();
    }

    // por primeira peca na primeira posicao do tabuleiro
    /*
    for(int i = 0; i < 4; i++)
    {
        cout << p.rot2[i] ;
    }
        cout << endl;
    for(int i = 0; i < 4; i++)
    {
        cout << p.rot3[i] ;
    }
        cout << endl;
    for(int i = 0; i < 4; i++)
    {
        cout << p.rot4[i] ;
    }
    cout << endl;
    */

    // lista.push_front(p);

    // int c = find_match((*lista.begin()).numbers,(*std::next(lista.begin(), 1)).numbers);
    // cout << "olha a match:" <<c << endl;
    // showlist(lista);

    return 0;
}