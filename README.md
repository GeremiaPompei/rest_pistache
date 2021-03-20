# rest_pistache

Programma utile per visualizzare il funzionamento dello scambio di messaggi tra *client* e *server* tramite protocollo REST in C++.

Per tale progetto sono state utilizzate le librerie [**pistache**](https://github.com/pistacheio/pistache) e [**rapidjson**](https://github.com/Tencent/rapidjson/)

## Istallazione e Build
### Istallazione Pistache

Ubuntu
```
sudo add-apt-repository ppa:pistache+team/unstable
sudo apt update
sudo apt install libpistache-dev
```
Debian
```
git clone https://github.com/pistacheio/pistache
git submodule update --init
cd pistache
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make -j
make install
```

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
L'eseguibile può essere lanciato con informazioni aggiuntive come il **numero** **di** **porta** dove far partire il server e il **numero** **di** **thread** 
per l'esecuzione del server. Di default essi sono rispettivamente **9080** e **2**. Per aggiungere tali informazioni basta aggiungerli al posto di N_PORT e 
N_THREAD in tale comando
```
./rest_pistache N_PORT N_THREAD
```

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
