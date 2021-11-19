#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <vector>
#include <map>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iomanip>
using namespace std;

#define MAXBlocos 25000

/* Vetor para meses */
string mes[13] = { "NULL", "jan", "fev", "mar", "abr", "mai", "jun", "jul", "ago", "set", "out", "nov", "dez" };

/* STRUCT para manipulação de diretórios e arquivos */
struct DirFile {
    int tipo;                                                       // 0 = Diretório | 1 = Arquivo | 2 = Continuação de Arquivo
    string nome;                                                    // Nome do diretório ou arquivo
    int tamanho;                                                    // Tamanho em bytes se for um arquivo
    int dia_cri, mes_cri, ano_cri, h_cri, min_cri, seg_cri;         // Data em que foi criado
    int dia_mod, mes_mod, ano_mod, h_mod, min_mod, seg_mod;         // Data em que foi modificado
    int dia_acc, mes_acc, ano_acc, h_acc, min_acc, seg_acc;         // Data em que foi acessado
    vector<int> fblocos;                                            // Primeiros blocos de memória dos arquivos contidos no diretório
    string conteudo;                                                // Conteúdo se for um arquivo
};

fstream sistemaArq;             // Arquivo montado
bool bitmap[MAXBlocos];         // Vetor BITMAP
int FAT[MAXBlocos];             // Vetor FAT
map<int, DirFile> DirFiles;     // Mapa de diretórios e arquivos (guarda o bloco de memória em que está localizado e conteúdo do diretório ou arquivo)

/* Mensagem de erro ao usuário quando não há blocos de memória disponíveis */
void displayError () {
    cout << "Não há blocos de memória disponíveis! Não foi possível realizar a operação!" << endl;
}

/* Comando para montagem de um sistema de arquivos com nome "nomeArquivo" */
void mountArquivo (string nomeArquivo) {
    int blocoMemAtual = 48;

    /* Retira o barra do começo de arquivo */
    if (nomeArquivo[0] == '/')
        nomeArquivo.erase (nomeArquivo.begin ());

    /* O arquivo foi aberto corretamente */
    sistemaArq.open (nomeArquivo, ios::in);
    if (!sistemaArq.fail ()) {
        string number;

        /* Lê e coloca o BITMAP e FAT nos vetores */
        for (int i = 0; i < MAXBlocos; i ++) {
            sistemaArq >> number;
            bitmap[i] = stoi (number);
        }

        for (int i = 0; i < MAXBlocos; i ++) {  
            sistemaArq >> number;
            FAT[i] = stoi (number);
        }

        /* Lê o restante do sistema de arquivos e os coloca dentro do programa */
        string type;
        while (sistemaArq >> type) {

            /* Se é um diretório */
            if (type.compare ("/") == 0) {
                DirFile diretorio;
                string aux;

                diretorio.tipo = 0;
                sistemaArq >> diretorio.nome;
                sistemaArq >> diretorio.dia_cri;    sistemaArq >> diretorio.mes_cri;    sistemaArq >> diretorio.ano_cri;
                sistemaArq >> diretorio.h_cri;      sistemaArq >> diretorio.min_cri;    sistemaArq >> diretorio.seg_cri;

                sistemaArq >> diretorio.dia_mod;    sistemaArq >> diretorio.mes_mod;    sistemaArq >> diretorio.ano_mod;
                sistemaArq >> diretorio.h_mod;      sistemaArq >> diretorio.min_mod;    sistemaArq >> diretorio.seg_mod;

                sistemaArq >> diretorio.dia_acc;    sistemaArq >> diretorio.mes_acc;    sistemaArq >> diretorio.ano_acc;
                sistemaArq >> diretorio.h_acc;      sistemaArq >> diretorio.min_acc;    sistemaArq >> diretorio.seg_acc;

                // Coloca os metadados dos arquivos presentes nesse diretório dentro do map
                string nomeArq;
                sistemaArq >> nomeArq;

                while (nomeArq.compare ("BLDIR") != 0) {
                    
                    // Lê o bloco do arquivo
                    string blocoArq;
                    sistemaArq >> blocoArq;
                    
                    // Coloca o blocoArq no fblocos do diretório
                    diretorio.fblocos.push_back (stoi (blocoArq));

                    /* Verifica se já existe um arquivo criado com conteúdo no bloco "blocoArq" */
                    map<int, DirFile>::iterator it = DirFiles.find (stoi (blocoArq));

                    // Se não existe, cria
                    if (it == DirFiles.end ()) {
                        DirFile arquivo;
                        arquivo.tipo = 1;

                        arquivo.nome = nomeArq;

                        sistemaArq >> aux;
                        arquivo.tamanho = stoi (aux);

                        sistemaArq >> arquivo.dia_cri;    sistemaArq >> arquivo.mes_cri;    sistemaArq >> arquivo.ano_cri;
                        sistemaArq >> arquivo.h_cri;      sistemaArq >> arquivo.min_cri;    sistemaArq >> arquivo.seg_cri;

                        sistemaArq >> arquivo.dia_mod;    sistemaArq >> arquivo.mes_mod;    sistemaArq >> arquivo.ano_mod;
                        sistemaArq >> arquivo.h_mod;      sistemaArq >> arquivo.min_mod;    sistemaArq >> arquivo.seg_mod;

                        sistemaArq >> arquivo.dia_acc;    sistemaArq >> arquivo.mes_acc;    sistemaArq >> arquivo.ano_acc;
                        sistemaArq >> arquivo.h_acc;      sistemaArq >> arquivo.min_acc;    sistemaArq >> arquivo.seg_acc;

                        DirFiles.insert (pair<int, DirFile> (stoi (blocoArq), arquivo));
                    }

                    // Se existe, atualiza metadados
                    else {
                        (it -> second).tipo = 1;
                        
                        (it -> second).nome = nomeArq;

                        sistemaArq >> aux;
                        (it -> second).tamanho = stoi (aux);

                        sistemaArq >> (it -> second).dia_cri;    sistemaArq >> (it -> second).mes_cri;    sistemaArq >> (it -> second).ano_cri;
                        sistemaArq >> (it -> second).h_cri;      sistemaArq >> (it -> second).min_cri;    sistemaArq >> (it -> second).seg_cri;

                        sistemaArq >> (it -> second).dia_mod;    sistemaArq >> (it -> second).mes_mod;    sistemaArq >> (it -> second).ano_mod;
                        sistemaArq >> (it -> second).h_mod;      sistemaArq >> (it -> second).min_mod;    sistemaArq >> (it -> second).seg_mod;

                        sistemaArq >> (it -> second).dia_acc;    sistemaArq >> (it -> second).mes_acc;    sistemaArq >> (it -> second).ano_acc;
                        sistemaArq >> (it -> second).h_acc;      sistemaArq >> (it -> second).min_acc;    sistemaArq >> (it -> second).seg_acc;
                    }

                    sistemaArq >> nomeArq;
                }

                // Coloca os blocos de memória ocupados por diretórios dentro desse diretório no fblocos
                while (sistemaArq.peek () != '\n') {
                    sistemaArq >> aux;
                    diretorio.fblocos.push_back (stoi (aux));
                }

                // Procura em qual bloco de memória o diretório se encontra
                for (int i = blocoMemAtual; i < MAXBlocos; i ++) {
                    if (bitmap[i] == 1) {
                        blocoMemAtual = i + 1;
                        break;
                    }
                }

                // Insere no MAP
                DirFiles.insert (pair<int, DirFile> (blocoMemAtual - 1, diretorio));
            }

            /* Se é continuação de um arquivo */
            else if (type.compare (">>") == 0) {
                DirFile contArquivo;
                string aux;
                contArquivo.tipo = 2;

                sistemaArq >> aux;
                contArquivo.conteudo = aux;

                // Procura em qual bloco de memória o arquivo se encontra
                for (int i = blocoMemAtual; i < MAXBlocos; i ++) {
                    if (bitmap[i] == 1) {
                        blocoMemAtual = i + 1;
                        break;
                    }
                }

                // Insere no MAP
                DirFiles.insert (pair<int, DirFile> (blocoMemAtual - 1, contArquivo));
            }

            /* Se é um arquivo */
            else {
                string aux;

                // Procura em qual bloco de memória o arquivo se encontra
                for (int i = blocoMemAtual; i < MAXBlocos; i ++) {
                    if (bitmap[i] == 1) {
                        blocoMemAtual = i + 1;
                        break;
                    }
                }
                
                /* Verifica se já existe um arquivo dentro do MAP */
                map<int, DirFile>::iterator it = DirFiles.find (blocoMemAtual - 1);

                // Se não existe, cria;
                if (it == DirFiles.end ()) {
                    DirFile arquivo;

                    arquivo.tipo = 1;

                    getline (sistemaArq, aux, '\n');
                    if (!aux.empty ())
                        arquivo.conteudo = type + " " + aux;
                    else
                        arquivo.conteudo = type;

                    // Insere no MAP
                    DirFiles.insert (pair<int, DirFile> (blocoMemAtual - 1, arquivo));
                }

                // Se existe, atualiza o conteúdo e o nome apenas
                else {
                    getline (sistemaArq, aux, '\n');
                    if (!aux.empty ())
                        (it -> second).conteudo = type + " " + aux;
                    else
                        (it -> second).conteudo = type;
                }
            }
        }

        // Necessário abrir o arquivo novamente em modo "trunc"
        sistemaArq.close ();
        sistemaArq.open (nomeArquivo, ios::out | ios::trunc);
    }

    /* O arquivo não existe, cria um sistema de arquivos do zero */
    else {
        sistemaArq.open (nomeArquivo, ios::out);

        /* Cria o BITMAP */
        // Blocos de memória reservados para BITMAP e FAT
        for (int i = 0; i < 48; i ++)
            bitmap[i] = 1;
        for (int i = 48; i < 25000; i ++)
            bitmap[i] = 0;

        /* Cria o FAT */
        for (int i = 0; i < 25000; i ++)
            FAT[i] = -1;

        /* Cria o diretório RAIZ no sistema de arquivos */
        DirFile raiz;
        raiz.tipo = 0;
        raiz.nome = "RAIZ";

        time_t now = time (0);
        tm *curr_date = localtime (&now);

        raiz.dia_cri = raiz.dia_mod = raiz.dia_acc = curr_date->tm_mday;
        raiz.mes_cri = raiz.mes_mod = raiz.mes_acc = 1 + curr_date->tm_mon;
        raiz.ano_cri = raiz.ano_mod = raiz.ano_acc = 1900 + curr_date->tm_year;
        raiz.h_cri = raiz.h_mod = raiz.h_acc = curr_date->tm_hour;
        raiz.min_cri = raiz.min_mod = raiz.min_acc = curr_date->tm_min;
        raiz.seg_cri = raiz.seg_mod = raiz.seg_acc = curr_date->tm_sec;

        DirFiles.insert (pair<int, DirFile> (48, raiz));
        bitmap[48] = 1;
    }
}

/* Comando para cópia de um arquivo para o sistema de arquivos */
void cp (string origem, string destino) {

    /* Lê o conteúdo do arquivo da origem */
    fstream arqLido;
    string conteudo;

    if (origem[0] == '/')
        origem.erase (origem.begin ());

    arqLido.open (origem, ios::in);
    ostringstream ss;
    ss << arqLido.rdbuf();
    conteudo = ss.str();

    /* Verifica se há espaço disponível para colocar o arquivo no sistema de arquivos */
    // Nome do arquivo
    string nomeArq = "";
    int aux;
    for (int i = destino.length () - 1; i >= 0; i --) {
        if (destino[i] == '/') {
            aux = i + 1;
            break;
        }
    }
    for (int i = aux; i < destino.length (); i ++)
        nomeArq += destino[i];

    // Calcula o tamanho necessário para acomodar o arquivo dentro do sistema de arquivos, incluindo metadados
    string algTamanhoTotal;
    int tamanhoTotal;
    tamanhoTotal = 69 + nomeArq.length () + conteudo.length ();
    algTamanhoTotal = to_string (tamanhoTotal);
    tamanhoTotal = tamanhoTotal + algTamanhoTotal.length ();
    if (to_string (tamanhoTotal).length () > algTamanhoTotal.length ())
        tamanhoTotal ++;
    
    int tamanhoNecessario;
    tamanhoNecessario = 1 + conteudo.length ();

    int tmp = tamanhoNecessario;
    int nBlocosNecessarios = 1;
    while (tmp / 4000 > 0) {
        tmp = tmp - 4000 + 4;
        nBlocosNecessarios ++;
    }

    tamanhoNecessario = tamanhoNecessario + (nBlocosNecessarios - 1) * 4;
    tamanhoTotal = tamanhoTotal + (nBlocosNecessarios - 1) * 4;

    // Seleciona quais blocos de memória deverão ser usados para acomodar o arquivo
    int nBlocosAtuais = 0;
    vector<int> blocosNecessarios;
    for (int i = 48; i < MAXBlocos && nBlocosAtuais != nBlocosNecessarios; i ++) {
        if (bitmap[i] == 0) {
            blocosNecessarios.push_back (i);
            nBlocosAtuais ++;
        }
    }

    // É possível acomodar o arquivo
    /* Coloca o conteúdo do arquivo lido no MAP */
    if (nBlocosAtuais == nBlocosNecessarios) {

        // Altera o Bitmap e o FAT
        for (auto i = blocosNecessarios.begin (); i != blocosNecessarios.end (); i ++) {
            auto proxBloco = i + 1;

            bitmap[*i] = 1;
            if (proxBloco != blocosNecessarios.end ())
                FAT[*i] = *proxBloco;
        }

        /* Coloca o arquivo principal */
        DirFile arquivo;
        arquivo.tipo = 1;
    
        // Nome
        arquivo.nome = nomeArq;

        // Tamanho do arquivo
        arquivo.tamanho = tamanhoTotal;

        // Datas do arquivo
        time_t now = time (0);
        tm *curr_date = localtime (&now);

        arquivo.dia_cri = arquivo.dia_mod = arquivo.dia_acc = curr_date->tm_mday;
        arquivo.mes_cri = arquivo.mes_mod = arquivo.mes_acc = 1 + curr_date->tm_mon;
        arquivo.ano_cri = arquivo.ano_mod = arquivo.ano_acc = 1900 + curr_date->tm_year;
        arquivo.h_cri = arquivo.h_mod = arquivo.h_acc = curr_date->tm_hour;
        arquivo.min_cri = arquivo.min_mod = arquivo.min_acc = curr_date->tm_min;
        arquivo.seg_cri = arquivo.seg_mod = arquivo.seg_acc = curr_date->tm_sec;

        // Conteúdo do arquivo
        int espacoOcupado = 1;
        int espacoLivre = 4000 - espacoOcupado;

        string backupConteudo = conteudo;
        if (nBlocosAtuais > 1) {
            conteudo.erase (conteudo.begin () + espacoLivre, conteudo.end ());
            nBlocosAtuais --;
        }
        arquivo.conteudo = conteudo;

        DirFiles.insert (pair<int, DirFile> (*blocosNecessarios.begin (), arquivo));

        /* Coloca o restante do arquivo no MAP em outros blocos de memória */
        for (auto i = blocosNecessarios.begin () + 1; i != blocosNecessarios.end (); i ++) {
            DirFile restArq;
            string conteudoDef;
            int espaco = 3996;

            // Tipo de arquivo (continuação de arquivo)
            restArq.tipo = 2;

            backupConteudo = backupConteudo.erase (0, espacoLivre);
            conteudoDef = backupConteudo;
            if (nBlocosAtuais > 1) {
                conteudoDef.erase (conteudoDef.begin () + espaco, conteudoDef.end ());
                nBlocosAtuais --;
            }
            espacoLivre = espaco;

            restArq.conteudo = conteudoDef;

            DirFiles.insert (pair<int, DirFile> (*i, restArq));
        }

        /* Adiciona no vetor fBlocos do diretório que conterá esse arquivo o seu primeiro bloco de conteúdo */
        char diretorio[destino.length () + 1];
        strcpy (diretorio, destino.c_str ());
        char *token = strtok (diretorio, "/");

        // O arquivo se encontra em outros diretórios (mas cuida do caso em que o arquivo se encontra na raíz)
        map<int, DirFile>::iterator dirMap = DirFiles.find (48);
        while (token != nomeArq) {
            vector<int> fblocosProv = (dirMap -> second).fblocos;

            for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
                dirMap = DirFiles.find (*i);

                if ( (dirMap -> second).tipo == 0 && (dirMap -> second).nome == token) {
                    token = strtok (NULL, "/");
                    break;
                }
            }
        }

        (dirMap -> second).fblocos.push_back (*blocosNecessarios.begin ());
    }

    // Não é possível acomodar o arquivo
    else
        displayError ();
}

/* Cria um diretório dentro do sistema de arquivos */
void mkdir (string caminho) {

    // Encontrar o próximo bloco de memória livre para alocar o diretório
    int blocoLivre = -1;
    for (int i = 48; i < MAXBlocos; i ++) {
        if (bitmap[i] == 0) {
            blocoLivre = i;
            break;
        }
    }

    // Verifica se existe espaço livre para acomodar o diretório
    if (blocoLivre != -1) {

        // Retira o barra do final
        if (caminho[caminho.length () - 1] == '/')
            caminho.erase (caminho.length () - 1);

        char diretorio[caminho.length () + 1];
        strcpy (diretorio, caminho.c_str ());
        char *token = strtok (diretorio, "/");
        string nomeDir = "";

        // Pega o nome do arquivo
        for (int i = caminho.length () - 1; i >= 0 && caminho[i] != '/'; i --) {
            char c;
            
            c = caminho[i];
            nomeDir = c + nomeDir;
        }

        // O diretório se encontra em outros diretórios (mas cuida do caso em que o diretório se encontra na raíz)
        map<int, DirFile>::iterator dirMap = DirFiles.find (48);
        while (token != nomeDir) {
            vector<int> fblocosProv = (dirMap -> second).fblocos;

            for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
                dirMap = DirFiles.find (*i);

                if ((dirMap -> second).nome == token) {
                    token = strtok (NULL, "/");
                    break;
                }
            }
        }

        /* Colocar o diretório criado dentro dos diretório necessários */
        DirFile dir;
        dir.tipo = 0;

        // Nome do diretório
        dir.nome = nomeDir;

        // Datas do diretório
        time_t now = time (0);
        tm *curr_date = localtime (&now);

        dir.dia_cri = dir.dia_mod = dir.dia_acc = curr_date->tm_mday;
        dir.mes_cri = dir.mes_mod = dir.mes_acc = 1 + curr_date->tm_mon;
        dir.ano_cri = dir.ano_mod = dir.ano_acc = 1900 + curr_date->tm_year;
        dir.h_cri = dir.h_mod = dir.h_acc = curr_date->tm_hour;
        dir.min_cri = dir.min_mod = dir.min_acc = curr_date->tm_min;
        dir.seg_cri = dir.seg_mod = dir.seg_acc = curr_date->tm_sec;

        // Atualizar o bitmap
        bitmap[blocoLivre] = 1;

        // Atualizar o fblocos do diretório em que se está o diretório criado
        (dirMap -> second).fblocos.push_back (blocoLivre);

        // Atualiza a data de modificação do diretório pai
        (dirMap -> second).dia_mod = curr_date->tm_mday;
        (dirMap -> second).mes_mod = 1 + curr_date->tm_mon;
        (dirMap -> second).ano_mod = 1900 + curr_date->tm_year;
        (dirMap -> second).h_mod = curr_date->tm_hour;
        (dirMap -> second).min_mod = curr_date->tm_min;
        (dirMap -> second).seg_mod = curr_date->tm_sec;

        // Colocar o diretório criado no MAP
        DirFiles.insert (pair<int, DirFile> (blocoLivre, dir));
    }

    // Não existe bloco de memória disponível
    else {
        displayError ();
        return;
    }
}

/* Chamada recursiva para remover o diretório e o conteúdo dentro dele */
void rmdirRec (int blocoDir) {
    map<int, DirFile>::iterator dirMap = DirFiles.find (blocoDir);

    /* Percorre todos os arquivos e diretórios presentes no diretório atual apontado por dirMap */
    vector<int> fblocosProv = (dirMap -> second).fblocos;
    for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
        map<int, DirFile>::iterator arqDir = DirFiles.find (*i);

        // Se for um arquivo, apaga ele
        if ( (arqDir -> second).tipo == 1) {
            int backupBloco = arqDir -> first;

            DirFiles.erase (backupBloco);
            bitmap[backupBloco] = 0;

            cout << "O arquivo " << (arqDir -> second).nome << " foi apagado!" << endl;

            while (FAT[backupBloco] != -1) {
                int tmp = FAT[backupBloco];
                FAT[backupBloco] = -1;
                backupBloco = tmp;

                arqDir = DirFiles.find (backupBloco);
                DirFiles.erase (backupBloco);
                bitmap[backupBloco] = 0;
            }
        }

        // Se for um diretório, chama recursivamente rmdirRec
        else {
            rmdirRec (arqDir -> first);
        }
    }

    // Ao final, é necessário deletar o diretório
    cout << "O diretório " << (dirMap -> second).nome << " foi apagado!" << endl;
    bitmap[dirMap -> first] = 0;
    DirFiles.erase (dirMap -> first);
}

/* Comando para apagar um diretório e tudo que estiver abaixo dele */
void rmdir (string caminho) {
    char diretorio[caminho.length () + 1];
    strcpy (diretorio, caminho.c_str ());
    char *token = strtok (diretorio, "/");
    string nomeDir = "";

    // Retira o barra do final
    if (caminho[caminho.length () - 1] == '/')
        caminho.erase (caminho.length () - 1);

    // Pega o nome do diretório
    for (int i = caminho.length () - 1; i >= 0 && caminho[i] != '/'; i --) {
        char c;
        
        c = caminho[i];
        nomeDir = c + nomeDir;
    }

    // Procura o bloco de memória em que se encontra o diretório
    map<int, DirFile>::iterator dirMap = DirFiles.find (48);
    map<int, DirFile>::iterator dirAntes;
    while (token != NULL) {
        if (token == nomeDir)
            dirAntes = dirMap;

        vector<int> fblocosProv = (dirMap -> second).fblocos;
        for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
            dirMap = DirFiles.find (*i);

            if ((dirMap -> second).nome == token) {
                token = strtok (NULL, "/");
                break;
            }
        }
    }

    // Atualiza as datas de modificação e último acesso do diretório pai
    time_t now = time (0);
    tm *curr_date = localtime (&now);

    (dirAntes -> second).dia_mod = (dirAntes -> second).dia_acc = curr_date->tm_mday;
    (dirAntes -> second).mes_mod = (dirAntes -> second).mes_acc = 1 + curr_date->tm_mon;
    (dirAntes -> second).ano_mod = (dirAntes -> second).ano_acc = 1900 + curr_date->tm_year;
    (dirAntes -> second).h_mod = (dirAntes -> second).h_acc = curr_date->tm_hour;
    (dirAntes -> second).min_mod = (dirAntes -> second).min_acc = curr_date->tm_min;
    (dirAntes -> second).seg_mod = (dirAntes -> second).seg_acc = curr_date->tm_sec;

    // Atualiza o fblocos do diretório em que o diretório removido está
    vector<int> fblocosProv = (dirAntes -> second).fblocos;
    vector<int>::iterator it;
    it = find (fblocosProv.begin (), fblocosProv.end (), dirMap -> first);
    fblocosProv.erase (it);

    (dirAntes -> second).fblocos = fblocosProv;

    /* Chama recursivamente para apagar os arquivos e diretórios dentro dele */
    rmdirRec (dirMap -> first);
}

/* Mostra o conteúdo do arquivo "arquivo" na tela */
void cat (string arquivo) {
    char diretorio[arquivo.length () + 1];
    strcpy (diretorio, arquivo.c_str ());
    char *token = strtok (diretorio, "/");
    string nomeArq = "";

    // Pega o nome do arquivo
    for (int i = arquivo.length () - 1; i >= 0 && arquivo[i] != '/'; i --) {
        char c;
        
        c = arquivo[i];
        nomeArq = c + nomeArq;
    }

    // O arquivo se encontra em outros diretórios (mas cuida do caso em que o arquivo se encontra na raíz)
    map<int, DirFile>::iterator dirMap = DirFiles.find (48);
    while (token != NULL) {
        vector<int> fblocosProv = (dirMap -> second).fblocos;

        for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
            dirMap = DirFiles.find (*i);

            if ((dirMap -> second).nome == token) {
                token = strtok (NULL, "/");
                break;
            }
        }
    }

    // Atualiza a data de acesso do arquivo
    time_t now = time (0);
    tm *curr_date = localtime (&now);

    (dirMap -> second).dia_acc = curr_date->tm_mday;
    (dirMap -> second).mes_acc = 1 + curr_date->tm_mon;
    (dirMap -> second).ano_acc = 1900 + curr_date->tm_year;
    (dirMap -> second).h_acc = curr_date->tm_hour;
    (dirMap -> second).min_acc = curr_date->tm_min;
    (dirMap -> second).seg_acc = curr_date->tm_sec;

    /* Pega o conteúdo todo do arquivo */
    string conteudoTotal = "";

    conteudoTotal += (dirMap -> second).conteudo;

    int bloco = dirMap -> first;
    while (FAT[bloco] != -1) {
        bloco = FAT[bloco];
        dirMap = DirFiles.find (bloco);

        conteudoTotal += (dirMap -> second).conteudo;
    }

    cout << conteudoTotal << endl;
}

/* Muda a data acessada de um arquivo se existir, cria um arquivo vazio se não existir */
void touch (string caminho) {
    char diretorio[caminho.length () + 1];
    strcpy (diretorio, caminho.c_str ());
    char *token = strtok (diretorio, "/");
    string nomeArq = "";

    // Pega o nome do arquivo
    for (int i = caminho.length () - 1; i >= 0 && caminho[i] != '/'; i --) {
        char c;
        
        c = caminho[i];
        nomeArq = c + nomeArq;
    }

    // Encontra o bloco de memória que contém o arquivo
    bool found = false;
    map<int, DirFile>::iterator dirMap = DirFiles.find (48);
    map<int, DirFile>::iterator dirAntes;
    while (token != NULL) {
        if (token == nomeArq)
            dirAntes = dirMap;

        vector<int> fblocosProv = (dirMap -> second).fblocos;
        vector<int>::iterator i;
        for (i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
            dirMap = DirFiles.find (*i);

            if ((dirMap -> second).nome == token) {
                token = strtok (NULL, "/");

                if (token == NULL)
                    found = true;
                break;
            }
        }

        if (i == fblocosProv.end ())
            token = strtok (NULL, "/");
    }

    // Se o arquivo foi encontrado
    if (found == true) {

        // Atualiza a data de acesso do arquivo
        time_t now = time (0);
        tm *curr_date = localtime (&now);

        (dirMap -> second).dia_mod = (dirMap -> second).dia_acc = curr_date->tm_mday;
        (dirMap -> second).mes_mod = (dirMap -> second).mes_acc = 1 + curr_date->tm_mon;
        (dirMap -> second).ano_mod = (dirMap -> second).ano_acc = 1900 + curr_date->tm_year;
        (dirMap -> second).h_mod = (dirMap -> second).h_acc = curr_date->tm_hour;
        (dirMap -> second).min_mod = (dirMap -> second).min_acc = curr_date->tm_min;
        (dirMap -> second).seg_mod = (dirMap -> second).seg_acc = curr_date->tm_sec;
    }

    // Se o arquivo não existe
    else {

        /* Cria o arquivo */
        // Procura um bloco de memória livre
        int blocoLivre = -1;
        for (int i = 48; i < MAXBlocos; i ++) {
            if (bitmap[i] == 0) {
                blocoLivre = i;
                break;
            }
        }

        // Verifica se é possível realizar a operação
        if (blocoLivre != -1) {
            DirFile arq;
            arq.tipo = 1;

            // Altera o fblocos do diretório em que o arquivo se encontra
            (dirAntes -> second).fblocos.push_back (blocoLivre);

            // Atualiza o BITMAP
            bitmap[blocoLivre] = 1;

            // Nome do arquivo
            arq.nome = nomeArq;

            // Tamanho do arquivo
            int espacoOcupado = 63 + nomeArq.length ();
            espacoOcupado = espacoOcupado + to_string (espacoOcupado).length ();

            arq.tamanho = espacoOcupado;

            // Datas do arquivo
            time_t now = time (0);
            tm *curr_date = localtime (&now);

            arq.dia_cri = arq.dia_mod = arq.dia_acc = curr_date->tm_mday;
            arq.mes_cri = arq.mes_mod = arq.mes_acc = 1 + curr_date->tm_mon;
            arq.ano_cri = arq.ano_mod = arq.ano_acc = 1900 + curr_date->tm_year;
            arq.h_cri = arq.h_mod = arq.h_acc = curr_date->tm_hour;
            arq.min_cri = arq.min_mod = arq.min_acc = curr_date->tm_min;
            arq.seg_cri = arq.seg_mod = arq.seg_acc = curr_date->tm_sec;

            DirFiles.insert (pair<int, DirFile> (blocoLivre, arq));
        }

        else {
            displayError ();
            return;
        }
    }
}

/* Remove um arquivo do sistema de arquivos com caminho "caminho" */
void rm (string caminho) {
    char diretorio[caminho.length () + 1];
    strcpy (diretorio, caminho.c_str ());
    char *token = strtok (diretorio, "/");
    string nomeArq = "";

    // Pega o nome do arquivo
    for (int i = caminho.length () - 1; i >= 0 && caminho[i] != '/'; i --) {
        char c;
        
        c = caminho[i];
        nomeArq = c + nomeArq;
    }

    // Encontra o bloco de memória que contém o arquivo
    map<int, DirFile>::iterator dirMap = DirFiles.find (48);
    map<int, DirFile>::iterator dirAntes;
    while (token != NULL) {
        if (token == nomeArq) 
            dirAntes = dirMap;

        vector<int> fblocosProv = (dirMap -> second).fblocos;

        for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
            dirMap = DirFiles.find (*i);

            if ((dirMap -> second).nome == token) {
                token = strtok (NULL, "/");
                break;
            }
        }
    }

    // Atualiza as datas de modificação e último acesso do diretório pai
    time_t now = time (0);
    tm *curr_date = localtime (&now);

    (dirAntes -> second).dia_mod = (dirAntes -> second).dia_acc = curr_date->tm_mday;
    (dirAntes -> second).mes_mod = (dirAntes -> second).mes_acc = 1 + curr_date->tm_mon;
    (dirAntes -> second).ano_mod = (dirAntes -> second).ano_acc = 1900 + curr_date->tm_year;
    (dirAntes -> second).h_mod = (dirAntes -> second).h_acc = curr_date->tm_hour;
    (dirAntes -> second).min_mod = (dirAntes -> second).min_acc = curr_date->tm_min;
    (dirAntes -> second).seg_mod = (dirAntes -> second).seg_acc = curr_date->tm_sec;

    /* Atualiza o fblocos do diretório em que o arquivo se encontra */
    vector<int> fblocosProv = (dirAntes -> second).fblocos;
    vector<int>::iterator it;
    it = find (fblocosProv.begin (), fblocosProv.end (), dirMap -> first);
    fblocosProv.erase (it);

    (dirAntes -> second).fblocos = fblocosProv;

    /* Deleta os elementos do MAP que contém o conteúdo do arquivo */
    int backupBloco = dirMap -> first;

    DirFiles.erase (backupBloco);
    bitmap[backupBloco] = 0;

    while (FAT[backupBloco] != -1) {
        int tmp = FAT[backupBloco];
        FAT[backupBloco] = -1;
        backupBloco = tmp;

        dirMap = DirFiles.find (backupBloco);
        DirFiles.erase (backupBloco);
        bitmap[backupBloco] = 0;
    }
}

/* Comando para listagem de arquivos e diretórios dentro de um determinado diretório */
void ls (string caminho) {
    char diretorio[caminho.length () + 1];
    strcpy (diretorio, caminho.c_str ());
    char *token = strtok (diretorio, "/");
    string nomeDir = "";

    // Retira o barra do final
    if (caminho[caminho.length () - 1] == '/')
        caminho.erase (caminho.length () - 1);

    // Pega o nome do diretório
    for (int i = caminho.length () - 1; i >= 0 && caminho[i] != '/'; i --) {
        char c;
        
        c = caminho[i];
        nomeDir = c + nomeDir;
    }

    // Procura o bloco de memória em que se encontra o diretório
    map<int, DirFile>::iterator dirMap = DirFiles.find (48);
    while (token != NULL) {
        vector<int> fblocosProv = (dirMap -> second).fblocos;

        for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
            dirMap = DirFiles.find (*i);

            if ((dirMap -> second).nome == token) {
                token = strtok (NULL, "/");
                break;
            }
        }
    }

    /* Exibição de arquivos e diretórios */
    vector<int> fblocosProv = (dirMap -> second).fblocos;

    for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
        map<int, DirFile>::iterator it = DirFiles.find (*i);
        DirFile arqDir = it -> second;

        // Se é um diretório
        if (arqDir.tipo == 0) {
            cout << "DIRETORIO: " << "\033[1m\033[34m" << arqDir.nome << "\033[0m" << endl;
        }

        // Se é um arquivo
        else {
            cout << "ARQUIVO:   " << arqDir.tamanho << " "; 
            cout << mes[arqDir.mes_mod] << " ";
            cout << setw(2) << setfill('0') << arqDir.dia_mod << " ";
            cout << setw(2) << setfill('0') << arqDir.h_mod << ":";
            cout << setw(2) << setfill('0') << arqDir.min_mod << " ";
            cout << arqDir.nome << endl;
        }
    }
}

/* Procura recursivamente por nomeArq */
void findRec (string caminho, int blocoDir, string nomeArq) {
    map<int, DirFile>::iterator dirMap = DirFiles.find (blocoDir);

    vector<int> fblocosProv = (dirMap -> second).fblocos;
    for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
        map<int, DirFile>::iterator arqDir = DirFiles.find (*i);
        
        // Se for um arquivo, verifica se é o arquivo procurado
        if ( (arqDir -> second).tipo == 1 && (arqDir -> second).nome == nomeArq)
            cout << caminho << "/" << nomeArq << endl;
        
        // Se for um diretório, chama recursivamente
        else if ( (arqDir -> second).tipo == 0) {
            string tmp = caminho + "/" + (arqDir -> second).nome;
            findRec (tmp, (arqDir -> first), nomeArq);
        }
    }
}

/* Encontra a partir do diretório passado, arquivos com nome "nomeArq" */
void find (string caminho, string nomeArq) {
    char diretorio[caminho.length () + 1];
    strcpy (diretorio, caminho.c_str ());
    char *token = strtok (diretorio, "/");
    string nomeDir = "";

    // Retira o barra do final
    if (caminho[caminho.length () - 1] == '/')
        caminho.erase (caminho.length () - 1);

    // Pega o nome do diretório
    for (int i = caminho.length () - 1; i >= 0 && caminho[i] != '/'; i --) {
        char c;
        
        c = caminho[i];
        nomeDir = c + nomeDir;
    }

    // Encontra o bloco de memória que contém o arquivo
    map<int, DirFile>::iterator dirMap = DirFiles.find (48);
    while (token != NULL) {
        vector<int> fblocosProv = (dirMap -> second).fblocos;

        for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
            dirMap = DirFiles.find (*i);

            if ((dirMap -> second).nome == token) {
                token = strtok (NULL, "/");
                break;
            }
        }
    }

    // Procuro o nomeArq no diretório
    findRec (caminho, dirMap -> first, nomeArq);
}

/* Comando df */
void df () {

    // Quantidade de diretórios, arquivos e espaço livre
    int quntDir = 0;
    int quntArq = 0;
    int espLivre = 0;

    // Cálculo de espaço em livre em bytes
    int cnt = 0;
    for (int i = 0; i < MAXBlocos; i ++)
        if (bitmap[i] == 0)
            cnt ++;
    espLivre = cnt * 4000;

    // Cálculo de espaço desperdiçado
    int espDesperd = 0;
    for (auto i = DirFiles.begin (); i != DirFiles.end (); i ++) {

        // Cálculo do espaço desperdiçado por um diretório
        if ( (i -> second).tipo == 0) {
            int espOcupadoDir = 69 + ((i -> second).nome).length ();
            vector<int> fblocosProv = (i -> second).fblocos;
            
            for (auto i = fblocosProv.begin (); i != fblocosProv.end (); i ++) {
                map<int, DirFile>::iterator it = DirFiles.find (*i);

                DirFile arqDir = it -> second;
                if (arqDir.tipo == 0) {
                    espOcupadoDir += 6;
                }

                else if (arqDir.tipo == 1) {
                    espOcupadoDir += 68 + arqDir.nome.length () + to_string (arqDir.tamanho).length ();
                }
            }
            
            quntDir ++;
            espDesperd += 4000 - espOcupadoDir - 1; 
        }

        // Cálculo do espaço desperdiçado por um arquivo
        else if ( (i -> second).tipo == 1) {
            int espacoOcupadoArq = (i -> second).conteudo.length () + 1;

            quntArq ++;
            espDesperd += 4000 - espacoOcupadoArq;
        }

        // Cálculo do espaço desperdiçado por continuação de arquivo
        else {
            int espacoOcupadoCont = (i -> second).conteudo.length () + 4;

            espDesperd += 4000 - espacoOcupadoCont;
        }
    }

    // Espaço desperdiçado pelo BITMAP e FAT
    espDesperd += 1999;
    int espOcupado = 0;
    for (int i = 0; i < MAXBlocos; i ++)
        espOcupado += to_string(FAT[i]).length () + 1;
    espDesperd += (35 * 4000) - espOcupado;

    cout << "QUANTIDADE DE DIRETORIOS: " << quntDir << endl;
    cout << "QUANTIDADE DE ARQUIVOS:   " << quntArq << endl;
    cout << "ESPACO LIVRE:             " << espLivre << endl;
    cout << "ESPACO DESPERDICADO:      " << espDesperd << endl;
}

/* Coloca todas as alterações no sistema de arquivos */
void umount () {

    /* Escrita dos vetores BITMAP e FAT no sistema de arquivos */
    for (int i = 0; i < MAXBlocos; i ++)
        sistemaArq << bitmap[i] << " ";
    sistemaArq << endl;

    for (int i = 0; i < MAXBlocos; i ++)
        sistemaArq << FAT[i] << " ";
    sistemaArq << endl;

    /* Escrita dos diretórios e arquivos no sistema de arquivos */
    for (auto i = DirFiles.begin (); i != DirFiles.end (); i ++) {
        DirFile bloco = (i -> second);

        // Se é um diretório
        if (bloco.tipo == 0) {
            sistemaArq << "/ " << bloco.nome << endl;

            sistemaArq << setw(2) << setfill('0') << bloco.dia_cri << " " << setw(2) << setfill('0') << bloco.mes_cri << " " << setw(4) << setfill('0') << bloco.ano_cri << " " << setw(2) << setfill('0') << bloco.h_cri << " " << setw(2) << setfill('0') << bloco.min_cri << " " << setw(2) << setfill('0') << bloco.seg_cri << endl;
            sistemaArq << setw(2) << setfill('0') << bloco.dia_mod << " " << setw(2) << setfill('0') << bloco.mes_mod << " " << setw(4) << setfill('0') << bloco.ano_mod << " " << setw(2) << setfill('0') << bloco.h_mod << " " << setw(2) << setfill('0') << bloco.min_mod << " " << setw(2) << setfill('0') << bloco.seg_mod << endl;
            sistemaArq << setw(2) << setfill('0') << bloco.dia_acc << " " << setw(2) << setfill('0') << bloco.mes_acc << " " << setw(4) << setfill('0') << bloco.ano_acc << " " << setw(2) << setfill('0') << bloco.h_acc << " " << setw(2) << setfill('0') << bloco.min_acc << " " << setw(2) << setfill('0') << bloco.seg_acc << endl;

            vector<int> blocosDir;
            for (auto j = bloco.fblocos.begin (); j != bloco.fblocos.end (); j ++) {
                map<int, DirFile>::iterator blocoProv = DirFiles.find (*j);
                
                if ( (blocoProv -> second).tipo == 1) {
                    sistemaArq << (blocoProv -> second).nome << " ";
                    sistemaArq << setw(5) << setfill ('0') << (*j) << endl;

                    sistemaArq << (blocoProv -> second).tamanho << endl;

                    sistemaArq << setw(2) << setfill('0') << (blocoProv -> second).dia_cri << " " << setw(2) << setfill('0') << (blocoProv -> second).mes_cri << " " << setw(4) << setfill('0') << (blocoProv -> second).ano_cri << " " << setw(2) << setfill('0') << (blocoProv -> second).h_cri << " " << setw(2) << setfill('0') << (blocoProv -> second).min_cri << " " << setw(2) << setfill('0') << (blocoProv -> second).seg_cri << endl;
                    sistemaArq << setw(2) << setfill('0') << (blocoProv -> second).dia_mod << " " << setw(2) << setfill('0') << (blocoProv -> second).mes_mod << " " << setw(4) << setfill('0') << (blocoProv -> second).ano_mod << " " << setw(2) << setfill('0') << (blocoProv -> second).h_mod << " " << setw(2) << setfill('0') << (blocoProv -> second).min_mod << " " << setw(2) << setfill('0') << (blocoProv -> second).seg_mod << endl;
                    sistemaArq << setw(2) << setfill('0') << (blocoProv -> second).dia_acc << " " << setw(2) << setfill('0') << (blocoProv -> second).mes_acc << " " << setw(4) << setfill('0') << (blocoProv -> second).ano_acc << " " << setw(2) << setfill('0') << (blocoProv -> second).h_acc << " " << setw(2) << setfill('0') << (blocoProv -> second).min_acc << " " << setw(2) << setfill('0') << (blocoProv -> second).seg_acc << endl;
                }

                else
                    blocosDir.push_back (*j);
            }

            sistemaArq << "BLDIR";
            if (!blocosDir.empty ())
                sistemaArq << " ";

            for (auto j = blocosDir.begin (); j != blocosDir.end (); j ++) {
                sistemaArq << setw(5) << setfill('0') << (*j);

                if ( (j + 1) != blocosDir.end ())
                    sistemaArq << " ";
            }

            sistemaArq << endl;
        }

        // Se é um arquivo
        else if (bloco.tipo == 1)
            sistemaArq << bloco.conteudo << endl;

        // Se é continuação de um arquivo
        else {
            sistemaArq << ">>" << endl;
            sistemaArq << bloco.conteudo << endl;
        }
    }

    /* Retira todo o conteúdo presente no programa */
    memset (bitmap, 0, sizeof (bitmap));
    memset (FAT, -1, sizeof (FAT));
    DirFiles.clear ();

    /* O sistema de arquivos é fechado */
    sistemaArq.close ();
}

int main (int argc, char **argv) {
    bool running = true;
    string command;

    /* Enquanto o programa executa */
    while (running) {
        cout << "\033[1m\033[32m" << "[ep3]: " << "\033[0m";
        cin >> command;

        if (command.compare ("mount") == 0) {
            string nomeArquivo;
            cin >> nomeArquivo;

            mountArquivo (nomeArquivo);
        }
        
        else if (command.compare ("cp") == 0) {
            string origem, destino;

            cin >> origem;
            cin >> destino;

            cp (origem, destino);
        }

        else if (command.compare ("mkdir") == 0) {
            string caminho;
            cin >> caminho;

            mkdir (caminho);
        }
        
        else if (command.compare ("rmdir") == 0) {
            string caminho;
            cin >> caminho;

            rmdir (caminho);
        }

        else if (command.compare ("cat") == 0) {
            string arquivo;
            cin >> arquivo;

            cat (arquivo);
        }
        
        else if (command.compare ("touch") == 0) {
            string caminho;
            cin >> caminho;

            touch (caminho);
        }
        
        else if (command.compare ("rm") == 0) {
            string caminho;
            cin >> caminho;

            rm (caminho);
        }
        
        else if (command.compare ("ls") == 0) {
            string caminho;
            cin >> caminho;

            ls (caminho);
        }
        
        else if (command.compare ("find") == 0) {
            string caminho, nomeArq;
            cin >> caminho;
            cin >> nomeArq;

            find (caminho, nomeArq);
        }

        else if (command.compare ("df") == 0) {
            df ();
        }
        
        else if (command.compare ("umount") == 0) {
            umount ();
        }

        else if (command.compare ("sai") == 0) {
            running = false;
        }
    }

    return 0;
}