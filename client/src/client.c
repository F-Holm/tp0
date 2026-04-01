#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	if (logger == NULL){
		log_error(logger, "¡No se pudo crear el logger!");
		abort();
	}

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if (config == NULL){
		log_error(logger, "¡No se pudo crear el config!");
		abort();
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

	if (config == NULL || valor == NULL || ip == NULL || puerto == NULL){
		log_error(logger, "¡Error al leer la configuración!");
		abort();
	}
	log_info(logger, "Valor: %s - ip: %s - puerto: %s", valor, ip, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	return log_create("tp0.log", "client", true, LOG_LEVEL_INFO);
}

t_config* iniciar_config(void)
{
	return config_create("cliente.config");
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (leido != NULL && strcmp(leido, "") != 0 )
	{
		log_info(logger, "text: %s", leido);

		free(leido);

		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (leido != NULL && strcmp(leido, "") != 0 )
	{
		agregar_a_paquete(paquete, leido, strlen(leido));

		free(leido);

		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
}
