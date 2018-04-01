#include "nrfLib.h"

uint8_t CH = 0x07;

//Инициализация nrf24l01
void nrfInit() {
  setR(DDRB, 0b110); //Установка пинов для CN и CSN на выход
  SCSN; //Установка начальных значений
  spi_init();//Инициализация SPI
}

void nrfConf() {
  nrfST;//Включение модуля
  _delay_ms(2);//Пауза ожидания раскачки резонатора nrf
  nrfEP(0b111111);//Включение труб
  nrfAW(0b10);//Установка длины адреса
  nrfRETR(0b101111);//Установка количества автоповторов и задержки между ними
  nrfRF(0b110);//Настройка скорости и мощности модуля приема передачи
  nrfDPL(0b111111);//Установка труб с динамической длиной нагрузки
  nrfFR(0b110);//Разрешение динамической длины пакета и автоподтверждения
  nrfCH(CH);//Установка канала работы
}

//Чтение регистра из nrf
uint8_t nrfR(uint8_t reg) {
  RCSN;//Выбираем устройство для работы по spi
  spiWR(R_REGISTER | (REGISTER_MASK & reg));//Передаем адрес регистра с маской чтения
  uint8_t result = spiWR(NOP);//Холостая передача для приема значения
  SCSN;//Останавливаем работу с устройством
  return result;
}

//Чтение статус регистра nrf
uint8_t nrfR() {
  RCSN;//Выбираем устройство для работы по spi
  uint8_t result = spiWR(NOP);//Холостая передача для чтения статуса
  SCSN;//Останавливаем работу с устройством
  return result;
}

//Запись в регистр nrf
void nrfW(uint8_t reg, uint8_t value) {//Регистр и значение которое следует записать
  RCSN;//Выбираем устройство для работы по spi
  spiWR(W_REGISTER | (REGISTER_MASK & reg));//Передаем адрес регистра с маской записи
  spiWR(value);//Передаем данные для записи
  SCSN;//Останавливаем работу с устройством
}

//Смена аддреса труб
void nrfSAD(uint32_t value, uint8_t num = 0) {
  if (num > 7)num = 0;
  RCSN;//Выбираем устройство для работы по spi
  spiWR(W_REGISTER | (RX_ADDR_P0 + num) ); //Передаем адрес регистра с маской записи
  spiWR(value);
  spiWR(value >> 8);
  spiWR(value >> 16);
  spiWR(value >> 24);
  spiWR(0);
  SCSN;//Останавливаем работу с устройством
}

uint8_t nrfSD(uint8_t* send) {
  RCSN;//Выбираем устройство для работы по spi
  spiWR(FLUSH_TX);//Включения режима передачи
  SCSN;//Останавливаем работу с устройством

  RCSN;//Выбираем устройство для работы по spi
  spiWR(W_TX_PAYLOAD);//Конфигурация на запись в буфер nrf
  for (uint8_t i = 0; i < 32; i++)spiWR(send[i]);//Запись в буфер nrf
  SCSN;//Останавливаем работу с устройством

  SCE;//Переход в режим приема/передачи
  _delay_us(10);
  RCE;//Завершение передачи

  while (!(nrfR() & 0x30))_delay_us(10);//ждать завершение передачи

  nrfCH(CH);//Перезапуск канал для сброса счетчкика
  nrfW(STATUS, nrfR());//Сброс статуса после передачи
  if ((nrfR(OBSERVE_TX) & 0xf) != 0xf) {
    RCSN;//Выбираем устройство для работы по spi
    spiWR(R_RX_PAYLOAD);//Чтение из буфера приема
    SCSN;//Останавливаем работу с устройством
    return 1;
  } else return 0;
}

uint8_t nrfSD(uint8_t* send, uint8_t* buf) {
  RCSN;//Выбираем устройство для работы по spi
  spiWR(FLUSH_TX);//Включения режима передачи
  SCSN;//Останавливаем работу с устройством

  RCSN;//Выбираем устройство для работы по spi
  spiWR(W_TX_PAYLOAD);//Конфигурация на запись в буфер nrf
  for (uint8_t i = 0; i < 32; i++)spiWR(send[i]);//Запись в буфер nrf
  SCSN;//Останавливаем работу с устройством

  SCE;//Переход в режим приема/передачи
  _delay_us(10);
  RCE;//Завершение передачи

  while (!(nrfR() & 0x30))_delay_us(10);//ждать завершение передачи

  nrfCH(CH);//Перезапуск канал для сброса счетчкика
  nrfW(STATUS, nrfR());//Сброс статуса после передачи
  if ((nrfR(OBSERVE_TX) & 0xf) != 0xf) {
    RCSN;//Выбираем устройство для работы по spi
    spiWR(R_RX_PAYLOAD);//Чтение из буфера приема
    for (uint8_t i = 0; i < 32; i++)buf[i] = spiWR(0);//Запись ответа в буфер отправления
    SCSN;//Останавливаем работу с устройством
    return 1;
  } else return 0;
}


void nrfRD(uint8_t* buf) {
  RCSN;
  spiWR(R_RX_PAYLOAD);
  for (uint8_t i = 0; i < 32; i++)buf[i] = spiWR(0);
  SCSN;
  nrfW(STATUS, nrfR());
}

void nrfRD(uint8_t* buf, uint8_t* send, uint8_t numAP) {
  RCSN;
  spiWR(R_RX_PAYLOAD);
  for (uint8_t i = 0; i < 32; i++)buf[i] = spiWR(0);
  SCSN;
  nrfW(STATUS, nrfR());
  RCSN;
  spiWR(W_ACK_PAYLOAD + numAP);
  for (uint8_t i = 0; i < 32; i++)spiWR(send[i]);
  SCSN;
}
