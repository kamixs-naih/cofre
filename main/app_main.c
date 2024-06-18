//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       _              //
//               _    _       _      _        _     _   _   _    _   _   _        _   _  _   _          //
//           |  | |  |_| |\| |_| |\ |_|   |\ |_|   |_| |_| | |  |   |_| |_| |\/| |_| |  |_| | |   /|    //    
//         |_|  |_|  |\  | | | | |/ | |   |/ | |   |   |\  |_|  |_| |\  | | |  | | | |_ | | |_|   _|_   //
//                                                                                       /              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*   Programa básico para controle da placa durante a Jornada da Programação 1
*   Permite o controle das entradas e saídas digitais, entradas analógicas, display LCD e teclado. 
*   Cada biblioteca pode ser consultada na respectiva pasta em componentes
*   Existem algumas imagens e outros documentos na pasta Recursos
*   O código principal pode ser escrito a partir da linha 86
*/

// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ioplaca.h"   // Controles das Entradas e Saídas digitais e do teclado
#include "lcdvia595.h" // Controles do Display LCD
#include "hcf_adc.h"   // Controles do ADC
#include "MP_hcf.h"   // Controles do ADC
#include "driver/adc.h" 

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------



// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------

static const char *TAG = "Placa";
static uint8_t entradas, saidas = 0; 
static int senha_digitada = 0, digitos = 0;
     int num1 = 0; //declaração do primeiro numero da senha
     int num2 = 0; //declaracão do segundo numero da senha
     int num3 = 0; //declaração do terceiro numero da senha
     int num4 = 0; //declaração do terceiro nume da senha
     int coluna = 0; //declaração da coluna em que aparecerá os caracteres
     int resultado = 0; //variavel que terá a função de mostrar o resultado da digitação
     char tecla;
     int controle = 0; //controle / ordem dos caracteres 
     char mostra[40]; // mostra os caracteres de forma organizada
     int apaga = 0; //apaga os caracteres
     int operador = 0; //
     int fecha = 0; //variavel que vai fecha a tampa
     int saida = 0;
     uint32_t pot; //variavel que lê o adc
     char *str;
     int senha = 2807; //variavel que indica asenha
     int tentativas = 3;
   

     //variáveis de controle de entradas e saídas


// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------

// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void mostra_lcd(void) //função que irá mostrar o valor do adc no display

{

sprintf(&mostra[0], "%ld", pot); //mostra o valpor do pot no lcd
lcd595_write(2,0,&mostra[0]);

}

void abrir_cofre(void) //função para abrir o cofre

{
    for (int i = 0; i< 2031; i=pot) 
    {
      rotacionar_DRV ( 1 , 10 , saida); //abre a tampa
      hcf_adc_ler(&pot); //le o potenciometro
      mostra_lcd(); //mosstra o valpor no lcd
      
    }
    
}

void fechar_cofre(void) //função para fechar o cofre
{
    hcf_adc_ler(&pot); //le o potenciomentro
   for (int i = pot; i> 301; i=pot)
 
   {   rotacionar_DRV (0 , 10, saida); // fecha a tampa 
        hcf_adc_ler(&pot); //le o potenciometro
        mostra_lcd(); //mosstra o valpor no lcd
        
    }
}


static void verificar_senha(void)
{
   
}   

static void restaurar_senha(void)  //função para restaurar senha quando ela estiver errada
{
    
    senha = 0; //se a senha for diferente de 2807 
    lcd595_clear(); //apaga a senha
    digitos = digitos+1; //digita a senha
    lcd595_write (1,0, "clique em 'c' "); //apareçerá no lcd para digitar novamente
    lcd595_write (2,0, " e tente dnv "); //apareçerá novamente as lacunas para colocar a senha
      if(digitos == 1) //controle dos caracteres, ou seja, a ordem de cada caractere. Sendo asim, esse será o segundo
        {
            lcd595_write(2,0, "[*][][][]"); //ao clicar no primeiro numero, aparece um asterisco
        }
        if(digitos == 2) //controle do segundo digito
        {
            lcd595_write(2,0, "[*][*][][]"); ////ao clicar no segundo numero, aparece o segundo asterisco
        }
        if(digitos == 3) //controle do terceiro digito
        {
            lcd595_write(2,0, "[*][*][*][]"); //ao clicar no terceiro numero, aparece o terceiro asterisco
        }
        if(digitos== 4) //controle do quarto digito
        {
    controle = 0; 
    coluna = 0;
}
}



void app_main(void)

{

    /////////////////////////////////////////////////////////////////////////////////////   Programa principal

   

    // a seguir, apenas informações de console, aquelas notas verdes no início da execução
    ESP_LOGI(TAG, "Iniciando...");
    ESP_LOGI(TAG, "Versão do IDF: %s", esp_get_idf_version());

    /////////////////////////////////////////////////////////////////////////////////////   Inicializações de periféricos (manter assim)
    
    // inicializar os IOs e teclado da placa
    ioinit();      
    entradas = io_le_escreve(saidas); // Limpa as saídas e lê o estado das entradas

    // inicializar o display LCD 
    lcd595_init();
    lcd595_write(1,1,"    cofre da   ");
    lcd595_write(2,1," Naih ");
    
    // Inicializar o componente de leitura de entrada analógica
    esp_err_t init_result = hcf_adc_iniciar();
    if (init_result != ESP_OK) {
        ESP_LOGE("MAIN", "Erro ao inicializar o componente ADC personalizado");
    }

    

    

    //delay inicial
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
    lcd595_clear();

        /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados
   

    DRV_init(6,7); //inicializa o motor de passo
   
    
        
    lcd595_write(1,1, " digite a senha ");   
   
    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1) {
    
    tecla = le_teclado(); // a cada tecla que clicar, ele vai ler e mostrar no lcd
    entradas = io_le_escreve(saidas); //ler a tecla pressionada
    //hcf_adc_ler(&pot); //le o valor do pot
    //*sprintf (&mostra, "%" PRIu32" ",pot);
    lcd595_write(1,1,  " mostra ");

    
    if(tecla>='0' && tecla <= '9' )  //se a tecla for 0 até 9 é um numero
    {                                                                                                                           
       
                //controle dos caracteres, ou seja, a ordem de cada caractere. Sendo asim, esse será o primeiro
              
        num1 = num1  * 10 + tecla - '0'; //numero que multiplicado dá o dobro
                     //codigo do primeiro caractere que aparecer
        digitos = digitos+1; //acrescenta o proximo digito
          

        if(digitos == 1) //controle dos caracteres, ou seja, a ordem de cada caractere. Sendo asim, esse será o segundo
        {
            lcd595_write(2,0, "[*][][][]"); //ao clicar no primeiro numero, aparece um asterisco
        }
        if(digitos == 2) //controle do segundo digito
        {
            lcd595_write(2,0, "[*][*][][]"); ////ao clicar no segundo numero, aparece o segundo asterisco
        }
        if(digitos == 3) //controle do terceiro digito
        {
            lcd595_write(2,0, "[*][*][*][]"); //ao clicar no terceiro numero, aparece o terceiro asterisco
        }
        if(digitos== 4) //controle do quarto digito
        {
            lcd595_write(2,0, "[*][*][*][*]"); ///ao clicar no quarto numero, aparece o quarto asterisco
            if(num1 == 2807) // se a senha for 2807 o cofre abre
            { 
                lcd595_write(1,0, " senha correta "); //se a senha estiver correta, apareçerá no lcd 
                abrir_cofre(); //se a senha estiver correta, o cofre irá abrir
                 vTaskDelay( 100 / portTICK_PERIOD_MS);
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 10.."); //aparece na tecla em quanto tempo o cofre irá fechar.
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 9..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 8..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 7..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 6..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 5..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 4..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 3..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 2..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechando 1..");
                 lcd595_clear();
                 lcd595_write(1,0, " fechado..");
                 lcd595_clear();

            
                 vTaskDelay( 100 / portTICK_PERIOD_MS); //
                 fechar_cofre();
            }

            else //ou se a senha nao for 2807 a senha será incorreta e não abrirá o cofre
            {
                lcd595_write(1,0, "senha incorreta"); //irá parecer no lcd que a enha está incorreta
                restaurar_senha(); //e ele irá restaurar a senha para coloca-la novamente
            }

            controle=0;

           
        }
    }

      if (tecla== 'C') //se a tecla for C
      
       { 

      lcd595_clear(); //irá limpar tudo isso abaixo 
    
       num1 = apaga;
       num2 = apaga;
       num3 = apaga;
       num4 = apaga;
       digitos = apaga;
       }
           
        
     

    else if (tecla == '/')
        
        { 

            controle = 1;

        }

          else if (tecla == 'x')
        { 
            fecha = 0;
            controle = 1;

        }
    
   
       
        
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        vTaskDelay(500 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
    }
   
    // caso erro no programa, desliga o módulo ADC
    hcf_adc_limpar();

    
    
 

}

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
   
   





