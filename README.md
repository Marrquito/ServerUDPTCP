# Servidor UDP/TCP básico em C 

Usando TCP e UDP
Este é um programa em C/C++ que implementa um servidor TCP que atende aos seguintes requisitos:

O servidor pode receber até 10 conexões simultâneas de clientes na porta TCP 9600.

Cada cliente envia uma mensagem contendo um valor inteiro.

O valor inteiro enviado pelo cliente representa a porta UDP na qual o cliente estará aguardando uma mensagem.

O servidor responde enviando uma mensagem contendo um número aleatório para a porta UDP informada pelo cliente.

Caso o cliente envie o valor 0, o servidor é encerrado.

Para testar o servidor, você pode usar as ferramentas telnet e netcat, ou implementar um cliente que atenda a esses requisitos. Certifique-se de executar o servidor antes de iniciar os testes com os clientes.