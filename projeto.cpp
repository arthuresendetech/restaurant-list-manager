/* Programa criado por:
 * @arthuresendetech
 * @AugustoFrancoDev
*/ 

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct restaurante {
  string nome;
  string endereco;
  int estrelas;
  string chef;
  int anoDeFundacao;
  string obs;
  bool ativo = true;
};

// FUNCAO DE ORDENAMENTO
void quicksort(restaurante *&vetor, int esquerda, int direita, string &ordenamento);

// FUNCAO DE BUSCA BINARIA 
int BuscaBinaria(restaurante vetor[], int inicio, int fim, string busca, string tipo, bool inserir);

// FUNCAO DE REGISTRAR RESTURANTE
void insercaoOrdenada(restaurante *&vetor, int &m, int &n, string ordenamento);

// FUNCOES DE LEITURA E ESCRITA DE ARQUIVO
void lerArquivo(restaurante *&vetor, int &m, int &n, ifstream &entrada);
void escritaDeArquivo(restaurante vetor[], int m);

// FUNCOES DE EXIBIR LISTAS E RESTAURANTE
void exibirRestaurante(restaurante *vetor, int i);
void exibirLista(restaurante *vetor, int m, bool crescente);
void exibirLista_ano(restaurante *vetor, int m, int ano);
void exibirLista_estrelas(restaurante *vetor, int m, int estrelas);

// FUNCAO DE CONVERTER STRING PARA INTEIRO
int safe_stoi(string entrada);

// FUNCOES DE MENU
void mostrarMenuPrincipal();
int lerOpcaoMenu();

// FUNCOES PARA APAGAR TEXTO DO TERMINAL
void apagarLinhas(int linhas);
void pressionarEnter(int linhas);

int main() {
  ifstream arquivoEntrada("restaurantes.csv");

  if (!arquivoEntrada.is_open()) {
    cout << "Nao foi possivel abrir o arquivo 'restaurantes.csv'.\n";
    cout << "Verifique se o arquivo esta na mesma pasta do programa.\n";
    return 0;
  }

  int capacidade = 40;
  int qtdeCadastrados = 0;

  restaurante *restaurantes = new restaurante[capacidade];

  // indica por qual campo o vetor esta ordenado no momento
  string campoOrdenado = "nome";

  // leitura inicial do CSV
  lerArquivo(restaurantes, qtdeCadastrados, capacidade, arquivoEntrada);

  // ordena por nome logo apos a leitura
  if (qtdeCadastrados > 0) {
    quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
  }

  bool continuar = true;

  while (continuar) {
    mostrarMenuPrincipal();
    int opcao = lerOpcaoMenu();

    switch (opcao) {

    case 1: { // LISTAR
      apagarLinhas(9);
      if (qtdeCadastrados == 0) {
        cout << "\nNao ha restaurantes cadastrados para listar.\n";
        pressionarEnter(8);
        break;
      }

      cout << "\nComo deseja exibir?\n";
      cout << "  1) Todos os restaurantes\n";
      cout << "  2) Apenas um intervalo de posicoes (ex.: 10 a 20)\n";
      cout << "Opcao: ";

      string entrada;
      getline(cin, entrada);

      apagarLinhas(4);
      if (entrada.empty()) {
        cout << "Nenhuma opcao informada. Listando todos em ordem crescente.\n";
        exibirLista(restaurantes, qtdeCadastrados, true);
        break;
      }

      int tipoListagem = safe_stoi(entrada);

      if (tipoListagem == 1) {
        cout << "\nExibir lista em:\n";
        cout << "  1) Ordem crescente\n";
        cout << "  2) Ordem decrescente\n";
        cout << "Opcao: ";

        string entradaOrdem;
        getline(cin, entradaOrdem);
        apagarLinhas(4);

        if (entradaOrdem.empty()) {
          cout << "Nenhuma opcao informada. Usando ordem crescente.\n";
          exibirLista(restaurantes, qtdeCadastrados, true);
          break;
        }

        int ordem = safe_stoi(entradaOrdem);
        bool crescente = (ordem != 2);

        exibirLista(restaurantes, qtdeCadastrados, crescente);
        break;
      }

      else if (tipoListagem == 2) {
        cout << "Informe a posicao inicial (0 a " << qtdeCadastrados - 1
             << "): ";
        string iniStr;
        getline(cin, iniStr);

        cout << "Informe a posicao final (0 a " << qtdeCadastrados - 1 << "): ";
        string fimStr;
        getline(cin, fimStr);

        apagarLinhas(2);

        if (iniStr.empty() || fimStr.empty()) {
          cout << "Valores invalidos. Operacao cancelada.\n";
          pressionarEnter(8);
          break;
        }

        int inicio = safe_stoi(iniStr);
        int fim = safe_stoi(fimStr);

        if (inicio < 0 || fim >= qtdeCadastrados || inicio > fim) {
          cout << "Intervalo invalido. Use valores entre 0 e "
               << qtdeCadastrados - 1 << ".\n";
          pressionarEnter(8);
          break;
        }

        cout << "---------------------------------------------\n";
        for (int i = inicio; i <= fim; i++) {
          exibirRestaurante(restaurantes, i);
          cout << "---------------------------------------------\n";
        }
        pressionarEnter(2);
        break;
      }

      else {
        cout << "Opcao invalida.\n";
        pressionarEnter(8);
      }

      break;
    }

    case 2: { // ORDENAR
      apagarLinhas(9);
      if (qtdeCadastrados == 0) {
        cout << "\nNao ha restaurantes carregados para ordenar.\n";
        pressionarEnter(8);
        break;
      }

      cout << "\nEscolha o criterio de ordenacao:\n";
      cout << "  1) Nome\n";
      cout << "  2) Endereco\n";
      cout << "  3) Estrelas\n";
      cout << "  4) Chef\n";
      cout << "  5) Ano de fundacao\n";
      cout << "Opcao: ";

      string entrada;
      getline(cin, entrada);
      apagarLinhas(8);

      if (entrada.empty()) {
        cout << "Nenhuma opcao foi informada. Ordenacao cancelada.\n";
        pressionarEnter(7);
        break;
      }

      int criterio = safe_stoi(entrada);

      switch (criterio) {
      case 1:
        campoOrdenado = "nome";
        quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        apagarLinhas(4);
        break;
      case 2:
        campoOrdenado = "endereco";
        quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        apagarLinhas(4);
        break;
      case 3:
        campoOrdenado = "estrelas";
        quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        apagarLinhas(4);
        break;
      case 4:
        campoOrdenado = "chef";
        quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        apagarLinhas(4);
        break;
      case 5:
        campoOrdenado = "ano";
        quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        apagarLinhas(4);
        break;
      default:
        cout << "Criterio invalido. Nenhuma alteracao feita.\n";
        pressionarEnter(7);
        break;
      }

      break;
    }

    case 3: { // BUSCAR
      apagarLinhas(9);
      if (qtdeCadastrados == 0) {
        cout << "\nNao ha restaurantes cadastrados para busca.\n";
        pressionarEnter(8);
        break;
      }

      cout << "\nBuscar por:\n";
      cout << "  1) Nome\n";
      cout << "  2) Chef\n";
      cout << "  3) Endereco\n";
      cout << "  4) Quantidade de estrelas\n";
      cout << "  5) Ano de fundacao\n";
      cout << "  Opcao: ";

      string entrada;
      getline(cin, entrada);
      apagarLinhas(7);

      if (entrada.empty()) {
        cout << "Nenhuma opcao escolhida. Retornando ao menu.\n";
        pressionarEnter(8);
        break;
      }

      int tipo = safe_stoi(entrada);

      cout << "Digite o termo de busca: ";
      string chave;
      getline(cin, chave);
      apagarLinhas(1);

      int pos = -1;

      if (tipo == 1) {
        if (campoOrdenado != "nome") {
          campoOrdenado = "nome";
          quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        }
        pos = BuscaBinaria(restaurantes, 0, qtdeCadastrados - 1, chave, campoOrdenado, false);
      } else if (tipo == 2) {
        if (campoOrdenado != "chef") {
          campoOrdenado = "chef";
          quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        }
        pos = BuscaBinaria(restaurantes, 0, qtdeCadastrados - 1, chave, campoOrdenado, false);
      } else if (tipo == 3) {
        if (campoOrdenado != "endereco") {
          campoOrdenado = "endereco";
          quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        }
        pos = BuscaBinaria(restaurantes, 0, qtdeCadastrados - 1, chave, campoOrdenado, false);
      } else if (tipo == 4) {
        exibirLista_estrelas(restaurantes, qtdeCadastrados, safe_stoi(chave));
      } else if (tipo == 5) {
        exibirLista_ano(restaurantes, qtdeCadastrados, safe_stoi(chave));
      } else {
        cout << "Esse tipo de busca nao e valido.\n";
        pressionarEnter(8);
      }

      if (pos >= 0 && pos < qtdeCadastrados && restaurantes[pos].ativo) {
        cout << "\n=== Restaurante encontrado ===\n";
        exibirRestaurante(restaurantes, pos);
        cout << "---------------------------------------------\n";
        pressionarEnter(17);
      } else if (tipo == 1 || tipo == 2 || tipo == 3) {
        cout << "Nenhum restaurante encontrado.\n";
        pressionarEnter(8);
      }

      break;
    }

    case 4: { // INSERIR
      apagarLinhas(9);
      if (campoOrdenado.empty())
        campoOrdenado = "nome";

      insercaoOrdenada(restaurantes, qtdeCadastrados, capacidade,
                       campoOrdenado);
      apagarLinhas(6);
      cout << "Novo restaurante inserido com sucesso.\n";
      pressionarEnter(7);
      break;
    }

    case 5: { // REMOCAO LOGICA
      apagarLinhas(9);
      if (qtdeCadastrados == 0) {
        cout << "\nNao ha registros para remover.\n";
        pressionarEnter(8);
        break;
      }

      cout << "\nRemover com base em:\n";
      cout << "  1) Nome\n";
      cout << "  2) Chef\n";
      cout << "Opcao: ";

      string entrada;
      getline(cin, entrada);

      apagarLinhas(5);

      if (entrada.empty()) {
        cout << "Nenhuma opcao informada. Operacao cancelada.\n";
        pressionarEnter(7);
        break;
      }

      int tipo = safe_stoi(entrada);

      cout << "Digite o valor para remocao: ";
      string chave;
      getline(cin, chave);

      int pos = -1;

      if (tipo == 1) {
        if (campoOrdenado != "nome") {
          campoOrdenado = "nome";
          quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        }
        pos = BuscaBinaria(restaurantes, 0, qtdeCadastrados - 1, chave, campoOrdenado, false);
        apagarLinhas(1);
      } else if (tipo == 2) {
        if (campoOrdenado != "chef") {
          campoOrdenado = "chef";
          quicksort(restaurantes, 0, qtdeCadastrados - 1, campoOrdenado);
        }
        pos = BuscaBinaria(restaurantes, 0, qtdeCadastrados - 1, chave, campoOrdenado, false);
        apagarLinhas(1);
      } else {
        apagarLinhas(1);
        cout << "Tipo invalido.\n";
      }

      if (pos >= 0 && pos < qtdeCadastrados) {
        restaurantes[pos].ativo = false;
        cout << "Restaurante marcado como inativo (remocao logica).\n";
      } else if (tipo == 1 || tipo == 2) {
        cout << "Restaurante nao localizado.\n";
      }

      pressionarEnter(7);

      break;
    }

    case 6: { // SALVAR
      apagarLinhas(9);
      escritaDeArquivo(restaurantes, qtdeCadastrados);
      cout << "Arquivo 'restaurantes.csv' atualizado com sucesso.\n";
      cout << "Pressioner 'ENTER' para prosseguir.\n";
      cin.get();
      apagarLinhas(7);
      break;
    }

    case 7: { // SAIR
      apagarLinhas(9);
      cout << "Finalizando o sistema. Ate logo!\n";
      continuar = false;
      break;
    }

    default:
      apagarLinhas(9);
      cout << "Opcao invalida. Tente novamente.\n";
      pressionarEnter(7);
    }
  }

  delete[] restaurantes;
  return 0;
}

// FUNCAO PARA APAGAR LINHAS DO TERMINAL
void apagarLinhas(int linhas) {
  for (int i = 0; i < linhas; ++i) {
    cout << "\033[A\033[2K";
  }
}

// FUNCAO PARA PROSSEGUIR APOS PRESSIONAR TECLA 'ENTER'
void pressionarEnter(int linhas) {
  cout << "Pressione 'ENTER' para prosseguir...\n";
  cin.get();
  apagarLinhas(linhas);
}

// FUNCAO DE CONVERTER STRING PARA INTEIRO
int safe_stoi(string entrada) {
  int numero = 0;
  int potencia = 1;

  for (int i = (int)entrada.size() - 1; i >= 0; i--) {
    int digito = entrada[i] - '0';
    numero += digito * potencia;
    potencia *= 10;
  }

  return numero;
}

// FUNCAO PARA EXIBIR O MENU PRINCIPAL
void mostrarMenuPrincipal() {
  cout << "\n============================================\n";
  cout << "     GERENCIADOR DE RESTAURANTES MICHELIN   \n";
  cout << "============================================\n";
  cout << "  1) Listar restaurantes\n";
  cout << "  2) Ordenar restaurantes\n";
  cout << "  3) Buscar restaurante\n";
  cout << "  4) Inserir novo restaurante\n";
  cout << "  5) Remover restaurante (remocao logica)\n";
  cout << "  6) Salvar alteracoes em arquivo\n";
  cout << "  7) Encerrar programa\n";
  cout << "--------------------------------------------\n";
  cout << "Informe a opcao desejada: ";
}

// FUNCAO PARA LER A OPCAO DO MENU
int lerOpcaoMenu() {
  string entrada;
  getline(cin, entrada);

  while (entrada.size() == 0) {
    apagarLinhas(1);
    cout << "Digite uma opcao valida: ";
    getline(cin, entrada);
  }

  return safe_stoi(entrada);
}

// FUNCAO PARA REDIMENSIONAR VETOR
void redimensionaVetor(restaurante *&vetor, int &n) {
  restaurante *temp = new restaurante[n + 5];
  copy(vetor, vetor + n, temp);
  delete[] vetor;
  vetor = temp;
  n += 5;
}

// FUNCAO PARA LER UMA LINHA DE ARQUIVO .CSV
void leituraDeLinha(restaurante *&vetor, string linha, int &m) {
  int quebra1 = linha.find(',');
  int quebra2 = linha.find(',', quebra1 + 1);
  int quebra3 = linha.find(',', quebra2 + 1);
  int quebra4 = linha.find(',', quebra3 + 1);
  int quebra5 = linha.find(',', quebra4 + 1);
  vetor[m].nome = linha.substr(0, quebra1);
  vetor[m].endereco = linha.substr(quebra1 + 1, quebra2 - quebra1 - 1);
  vetor[m].estrelas =
      safe_stoi(linha.substr(quebra2 + 1, quebra3 - quebra2 - 1));
  vetor[m].chef = linha.substr(quebra3 + 1, quebra4 - quebra3 - 1);
  vetor[m].anoDeFundacao =
      safe_stoi(linha.substr(quebra4 + 1, quebra5 - quebra4 - 1));
  vetor[m].obs = linha.substr(quebra5 + 1);
  m++;
}

// FUNCAO PARA LER ARQUIVO .CSV
void lerArquivo(restaurante *&vetor, int &m, int &n, ifstream &entrada) {
  string linha;

  // pula titulo e cabecalo
  getline(entrada, linha);
  getline(entrada, linha);

  while (getline(entrada, linha)) {
    if (m >= n)
      redimensionaVetor(vetor, n);

    leituraDeLinha(vetor, linha, m);
  }

  entrada.close();
}

// FUNCAO PARA EXIBIR RESTAURANTE
void exibirRestaurante(restaurante *vetor, int i) {
  if (vetor[i].ativo) {
    cout << "Nome: " << vetor[i].nome << "\n";
    cout << "Indice: " << i << "\n";
    cout << "Endereco: " << vetor[i].endereco << "\n";
    cout << "Estrelas: " << vetor[i].estrelas << "\n";
    cout << "Chef: " << vetor[i].chef << "\n";
    cout << "Ano de fundacao: " << vetor[i].anoDeFundacao << "\n";
    cout << "Obs: " << vetor[i].obs << "\n";
  }
}

//------------------FUNCOES DE EXIBIR
//RESTAURANTES-------------------------------
void exibirLista(restaurante *vetor, int m, bool crescente) {
  cout << "---------------------------------------------\n";
  if (crescente) {
    for (int i = 0; i < m; i++) {
      exibirRestaurante(vetor, i);
      cout << "---------------------------------------------\n";
    }
  } else {
    for (int i = m - 1; i >= 0; i--) {
      exibirRestaurante(vetor, i);
      cout << "---------------------------------------------\n";
    }
  }
  pressionarEnter(2);
}

void exibirLista_ano(restaurante *vetor, int m, int ano) {
  bool encontrado = false;
  cout << "---------------------------------------------\n";
  for (int i = 0; i < m; i++) {
    if (vetor[i].anoDeFundacao == ano) {
      exibirRestaurante(vetor, i);
      encontrado = true;
      cout << "---------------------------------------------\n";
    }
  }
  if (!encontrado)
    cout << "Nenhum restaurante encontrado.\n";
  pressionarEnter(2);
}

void exibirLista_estrelas(restaurante *vetor, int m, int estrelas) {
  bool encontrado = false;
  cout << "---------------------------------------------\n";
  for (int i = 0; i < m; i++) {
    if (vetor[i].estrelas == estrelas) {
      exibirRestaurante(vetor, i);
      encontrado = true;
      cout << "---------------------------------------------\n";
    }
  }
  if (!encontrado)
    cout << "Nenhum restaurante encontrado.\n";
  pressionarEnter(2);
}

// FUNCAO PARA REGISTRAR UM RESTAURANTE
void registrarRestaurante(restaurante *&vetor, int &m) {
  string linha;

  cout << "Nome do novo restaurante: ";
  getline(cin, linha);
  vetor[m].nome = linha;

  cout << "Endereco do novo restaurante: ";
  getline(cin, linha);
  vetor[m].endereco = linha;

  cout << "Quantidade de estrelas do novo restaurante: ";
  getline(cin, linha);
  vetor[m].estrelas = safe_stoi(linha);

  cout << "Chef do novo restaurante: ";
  getline(cin, linha);
  vetor[m].chef = linha;

  cout << "Ano de fundacao do novo restaurante: ";
  getline(cin, linha);
  vetor[m].anoDeFundacao = safe_stoi(linha);

  cout << "Observacoes sobre o novo restaurante: ";
  getline(cin, linha);
  vetor[m].obs = linha;

  vetor[m].ativo = true;
  m++;
}

// FUNCAO PARA ESCREVER E SALVAR EM ARQUIVO .CSV
void escritaDeArquivo(restaurante vetor[], int m) {
  ofstream saida("restaurantes.csv");
  saida << "Restaurantes_ano_atualizado\n";
  saida << "Restaurante,Endereco,Estrelas,Chef,AnoDeFundacao,Obs\n";
  for (int i = 0; i < m; i++) {
    if (vetor[i].ativo) {
      saida << vetor[i].nome << ',' << vetor[i].endereco << ','
            << vetor[i].estrelas << ',' << vetor[i].chef << ','
            << vetor[i].anoDeFundacao << ',' << vetor[i].obs << "\n";
    }
  }
  saida.close();
}

// ----------------- QUICKSORT ----------------------------------------

bool comparaParticao (restaurante *vetor, string ordenamento, int indice, int pivo) {
  if (ordenamento == "nome") {
    return vetor[indice].nome < vetor[pivo].nome;
  }
  else if (ordenamento == "estrelas") {
    return vetor[indice].estrelas < vetor[pivo].estrelas;
  }
  else if (ordenamento == "chef") {
    return vetor[indice].chef < vetor[pivo].chef;
  }
  else if (ordenamento == "endereco") {
    return vetor[indice].endereco < vetor[pivo].endereco;
  }
  else if (ordenamento == "ano") {
    return vetor[indice].anoDeFundacao < vetor[pivo].anoDeFundacao;
  }
  return false;
}

int particao(restaurante *&vetor, int esquerda, int direita, string ordenamento) {
   int i = esquerda - 1;

  for (int j = esquerda; j < direita; j++) {
    if (comparaParticao(vetor, ordenamento, j, direita)) {
      i++;
      swap(vetor[i], vetor[j]);
    }
  }
  swap(vetor[i + 1], vetor[direita]);
  return i + 1;
}

void quicksort(restaurante *&vetor, int esquerda, int direita,
                        string &ordenamento) {
  if (esquerda < direita) {
    int pivo = particao(vetor, esquerda, direita, ordenamento);
    quicksort(vetor, esquerda, pivo - 1, ordenamento);
    quicksort(vetor, pivo + 1, direita, ordenamento);
  }
}

// ----------------- BUSCAS BINARIA -----------------------------------
bool comparaTipo (restaurante *vetor, string tipo, int indice, string busca, bool igual) {
  if (tipo == "nome") {
    if (igual) return vetor[indice].nome == busca;
    else return vetor[indice].nome < busca;
  }
  else if (tipo == "estrelas") {
    if (igual) return vetor[indice].estrelas == safe_stoi(busca);
    else return vetor[indice].estrelas < safe_stoi(busca);
  }
  else if (tipo == "chef") {
    if (igual) return vetor[indice].chef == busca;
    else return vetor[indice].chef < busca;
  }
  else if (tipo == "endereco") {
    if (igual) return vetor[indice].endereco == busca;
    else return vetor[indice].endereco < busca;
  }
  else if (tipo == "ano") {
    if (igual) return vetor[indice].anoDeFundacao == stoi(busca);
    else return vetor[indice].anoDeFundacao < safe_stoi(busca);
  }
  return false;
}

int BuscaBinaria(restaurante vetor[], int inicio, int fim, string busca, string tipo, bool inserir) {
    int meio = (inicio + fim) / 2;
  if (comparaTipo(vetor, tipo, meio, busca, true)) {
    return meio;
  } else if (inicio < fim) {
    if (comparaTipo(vetor, tipo, meio, busca, false))
      return BuscaBinaria(vetor, meio + 1, fim, busca, tipo, inserir);
    else
      return BuscaBinaria(vetor, inicio, meio - 1, busca, tipo, inserir);
  } else {
    if (inserir) return meio;
    else return -1;
  }
}


// FUNCAO DE INSERCAO ORDENADA
void insercaoOrdenada(restaurante *&vetor, int &m, int &n, string ordenamento) {
  if (m >= n)
    redimensionaVetor(vetor, n);

  registrarRestaurante(vetor, m);

  restaurante registrando = vetor[m - 1];
  int posicao = m - 1;
  
  if (ordenamento == "nome") {
    posicao = BuscaBinaria(vetor, 0, m - 1, registrando.nome, ordenamento, true);
  }
  else if (ordenamento == "chef") {
    posicao = BuscaBinaria(vetor, 0, m - 1, registrando.chef, ordenamento, true);
  }
   else if (ordenamento == "endereco") {
    posicao = BuscaBinaria(vetor, 0, m - 1, registrando.endereco, ordenamento, true);
  }
   else if (ordenamento == "ano") {
    posicao = BuscaBinaria(vetor, 0, m - 1, to_string(registrando.anoDeFundacao), ordenamento, true);
  }
   else if (ordenamento == "estrelas") {
    posicao = BuscaBinaria(vetor, 0, m - 1, to_string(registrando.estrelas), ordenamento, true);
  }
  
  for (int i = m - 1; i > posicao; i--) {
    vetor[i] = vetor[i - 1];
  }

  vetor[posicao] = registrando;
}
