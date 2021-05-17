# rest_pistache

Programma utile per visualizzare il funzionamento dello scambio di messaggi tra *client* e *server* tramite protocollo REST in C++.

Per tale progetto sono state utilizzate le librerie [**pistache**](https://github.com/pistacheio/pistache) e [**rapidjson**](https://github.com/Tencent/rapidjson/)

## Istallazione e Build
### Istallazione Pistache

Seguire README del progetto [**github**](https://github.com/pistacheio/pistache).

### Istallazione Rapidjson

```
sudo apt-get update -y
sudo apt-get install -y rapidjson-dev
```

### Build
Scaricato il programma in locale e istallate le dipendenze necessarie bisogna creare l'eseguibile tramite *cmake*.
Per scaricare cmake basta eseguire il comando
```
sudo apt-get install cmake
```
Dopo essersi assicurtati di avere cmake basta spostarsi sulla cartella principale del programma e creare al suo interno un ulteriore cartella chiamata **build** 
dentro alla quale verranno creati i file per la generazione dell'eseguibile. Per fare ciò basta lanciare i comandi
```
mkdir build
cd build
cmake ..
make
```
Creato l'eseguibile basta lanciarlo con il comando
```
./rest_pistache
```
Se non funziona probabilmente è perché non gli sono stati forniti i privilegi di esecuzione, per fare ciò
```
chmod +x rest_pistache
```

## SSL
Per abilitare il server a funzionare con SSL basta postporre al comando per far partire il programma la stringa **on**
```
./rest_pistache on
```
**NB**: SSL funziona solo per il server e non per il client. Per verificare se funziona il server basta andare sul browser e provare a connettersi importando il certificato **client.p12** nella cartella *client*. Vi è uno script linux per generare le chiavi e i certificati con OpenSSL chiamato **generate_key.sh**.

## Funzionamento
Lanciato il programma esso mette in ascolto il server che stampa i propri messaggi e contemporaneamente mette a disposizione la console per i comandi del client.
Il client tramite questi può creare, visualizzare ed eliminare nel server oggetti di tipo **Element** che hanno come attributi **id**, **name**, **description**.
Tali comandi hanno una struttura ben precisa, ovvero
```
http://indirizzo_ip_server:numero_porta_server/ metodo id_element [name_element description_element]
```
Il metodo può essere:
- **POST** crea un Element nel server con le caratteristiche inviate.
- **GET** visualizza un Element dal server tramite un certo id.
- **DEL** elimina un Element dal server con un certo id.

*name_element* e *description_element* sono tra parentesi quadre poichè vengono inseriti solo per una richiesta co metodo **POST**.

Per terminare l'esecuzione del *client* basta dare il comando **exit**.

Per terminare l'esecuzione del *server* e dell'intero programma basta premere CTRL C.
