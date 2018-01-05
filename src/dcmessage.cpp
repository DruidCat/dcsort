#include "dcmessage.h"
#include <QDebug>
#include <QMessageBox>

DCMessage::DCMessage(QLabel* plblStatusBar, int ntVremya, QObject *proditel) : QObject(proditel){
	/////////////////////////////////////////
	/////С Т Р О К А   С О С Т О Я Н И Я/////
	/////////////////////////////////////////
	m_strImyaKlassa = "";//Пустое имя класса
	if(proditel)//Если передан указатель класса, в котором будет работать класс сообщений, то...
		m_strImyaKlassa=proditel->metaObject()->className();//Присваиваем имя класса,в котором появилось сообщ

	m_plblStatusBar = plblStatusBar;//Присваеваем указателю лэйбл строчки отображения текста.

	m_ntSS = ntVremya;//Задаём параметр времени, которое будет отображать текст. 
	m_ptmrSS = new QTimer(this);//Указатель на таймер
	connect( 	m_ptmrSS,
				SIGNAL(timeout()),
				this,
				SLOT(slotSS()));//Связываем таймаут таймера со слотом.
}

DCMessage::DCMessage(QStatusBar* psbrStatusBar, int ntVremya, QObject *proditel) : QObject(proditel){
	/////////////////////////////////////////
	/////С Т Р О К А   С О С Т О Я Н И Я/////
	/////////////////////////////////////////
	m_strImyaKlassa = "";//Пустое имя класса
	if(proditel)//Если передан указатель класса, в котором будет работать класс сообщений, то...
		m_strImyaKlassa=proditel->metaObject()->className();//Присваиваем имя класса,в котором появилось сообщ

	m_psbrStatusBar = psbrStatusBar;//Присваеваем указателю лэйбл строчки отображения текста.

	m_ntSS = ntVremya;//Задаём параметр времени, которое будет отображать текст. 
	m_ptmrSS = new QTimer(this);//Указатель на таймер
	connect( 	m_ptmrSS,
				SIGNAL(timeout()),
				this,
				SLOT(slotSS()));//Связываем таймаут таймера со слотом.
}

DCMessage::~DCMessage(){//Деструктор.
/////////////////////////////
//---Д Е С Т Р У К Т О Р---//
/////////////////////////////
	m_psbrStatusBar = 0;//Обнуляем указатель.
	m_plblStatusBar = 0;//Обнуляем указатель.
	m_plblZagolovokOdin = 0;//Обнуляем указатель.
	m_plblZagolovokDva = 0;//Обнуляем указатель.
	delete m_ptmrSS;//Удаляем динамический указатель.
	m_ptmrSS = 0;//Обнуляем указатель.
}

void DCMessage::warning(QLabel* plblZagolovok, QString strSoobchenie){//Предупреждение с подсветкой заголовка.
/////////////////////////////////////
//---П Р Е Д У П Р Е Ж Д Е Н И Е---//
/////////////////////////////////////
	statusbar(strSoobchenie);//Обнуляем указатель заголовка, запускаем таймер и отображаем сообщение.
	m_plblZagolovokOdin = plblZagolovok;//Присваиваю адрес указателю.
	plblZagolovok->setStyleSheet("color: rgb(255, 0, 0)");//Подсвечиваем красным заголовок.
	QMessageBox msgBox;//Создаем объект Сообщения
	msgBox.setWindowTitle(plblZagolovok->text());//Устанавливаем заголовок окна сообщения.
	msgBox.setText(strSoobchenie);//Добавляем текст
	msgBox.setIcon(QMessageBox::Warning);//Устанавливаем иконку в сообщении.
	msgBox.exec();//Запускаем Сообщение
}

void DCMessage::warning(QLabel* plblZagolovokOdin, QLabel* plblZagolovokDva, QString strSoobchenie){
/////////////////////////////////////
//---П Р Е Д У П Р Е Ж Д Е Н И Е---//
/////////////////////////////////////
	statusbar(strSoobchenie);//Обнуляем указатель заголовка, запускаем таймер и отображаем сообщение.
	m_plblZagolovokOdin = plblZagolovokOdin;//Присваиваю адрес указателю.
	m_plblZagolovokDva = plblZagolovokDva;//Присваиваю адрес указателю.
	plblZagolovokOdin->setStyleSheet("color: rgb(255, 0, 0)");//Подсвечиваем красным заголовок.
	plblZagolovokDva->setStyleSheet("color: rgb(255, 0, 0)");//Подсвечиваем красным заголовок.
	QMessageBox msgBox;//Создаем объект Сообщения
	msgBox.setWindowTitle(plblZagolovokOdin->text());//Устанавливаем заголовок сообщения.
	msgBox.setText(strSoobchenie);//Добавляем текст
	msgBox.setIcon(QMessageBox::Warning);//Устанавливаем иконку в сообщении.
	msgBox.exec();//Запускаем Сообщение
}

void DCMessage::warning(QString strZagolovok, QString strSoobchenie){//Предупреждение без подсветки заголовка.
/////////////////////////////////////
//---П Р Е Д У П Р Е Ж Д Е Н И Е---//
/////////////////////////////////////
	statusbar(strSoobchenie);//Обнуляем указатель заголовка, запускаем таймер и отображаем сообщение.
	QMessageBox msgBox;//Создаем объект Сообщения
	msgBox.setWindowTitle(strZagolovok);//Устанавливаем заголовок окна сообщения.
	msgBox.setText(strSoobchenie);//Добавляем текст
	msgBox.setIcon(QMessageBox::Warning);//Устанавливаем иконку в сообщении.
	msgBox.exec();//Запускаем Сообщение
}

void DCMessage::information(ENMessage enmes, QString strSoobchenie){//Информация
/////////////////////////////
//---И Н Ф О Р М А Ц И Я---//
/////////////////////////////
	statusbar(strSoobchenie);//Обнуляем указатель заголовка, запускаем таймер и отображаем сообщение.

	QString strZagolovok("");//Заголовок сообщения.
	if ((enmes == izmenit)||(enmes == parol)||(enmes == vozrast)||(enmes == grafik)||(enmes == nedelya)
			||(enmes == telefon)||(enmes == strana)||(enmes == shrift))
		strZagolovok = tr("Сохранить:");//Устанавливаем заголовок окна сообщения.
	else{
		if(enmes == udalit)
			strZagolovok = tr("Удалить:");//Устанавливаем заголовок окна сообщения.
		else{
			if(enmes == sozdat)
				strZagolovok = tr("Создать:");//Устанавливаем заголовок окна сообщения.
			else{
				if(enmes == registraciya)
					strZagolovok = tr("Регистрация:");//Установить заголовок окна сообщения
				else{
					if(enmes == vhod)
						strZagolovok = tr("Вход:");
				}
			}
		}
	}

	QMessageBox msgBox;//Создаем объект Сообщения
	msgBox.setWindowTitle(strZagolovok);//Устанавливаем заголовок окна сообщения.
	msgBox.setText(strSoobchenie);//Добавляем текст
	msgBox.setIcon(QMessageBox::Information);//Устанавливаем иконку в сообщении.
	msgBox.exec();//Запускаем Сообщение
}

void DCMessage::information(ENMessage enmes, QStringList strlLog, QString strSoobchenie){//Инфо. в лог
/////////////////////////////
//---И Н Ф О Р М А Ц И Я---//
/////////////////////////////
	dcinfo(QStringList()<<m_strImyaKlassa<<QString::number(enmes)<<strlLog);//Отправляем сообщение в инфо лог.
	information(enmes, strSoobchenie);
}

void DCMessage::critical(QLabel* plblZagolovok, QString strSoobchenie){//Ошибка с подсветкой заголовка.
/////////////////////
//---О Ш И Б К А---//
/////////////////////
	statusbar(strSoobchenie);//Обнуляем указатель заголовка, запускаем таймер и отображаем сообщение.
	m_plblZagolovokOdin = plblZagolovok;//Присваиваю адрес указателю.
	plblZagolovok->setStyleSheet("color: rgb(255, 0, 0)");//Подсвечиваем красным заголовок.
	QMessageBox msgBox;//Создаем объект Сообщения
	msgBox.setWindowTitle(plblZagolovok->text());//Устанавливаем заголовок окна сообщения.
	msgBox.setText(strSoobchenie);//Добавляем текст
	msgBox.setIcon(QMessageBox::Critical);//Устанавливаем иконку в сообщении.
	msgBox.exec();//Запускаем Сообщение
}

void DCMessage::critical(QString strZagolovok, QString strSoobchenie){//Ошибка в виде сообщения.
/////////////////////
//---О Ш И Б К А---//
/////////////////////
	statusbar(strSoobchenie);//Обнуляем указатель заголовка, запускаем таймер и отображаем сообщение.
	QMessageBox msgBox;//Создаем объект Сообщения
	msgBox.setWindowTitle(strZagolovok);//Устанавливаем заголовок окна сообщения.
	msgBox.setText(strSoobchenie);//Добавляем текст
	msgBox.setIcon(QMessageBox::Critical);//Устанавливаем иконку в сообщении.
	msgBox.exec();//Запускаем Сообщение
}

void DCMessage::statusbar(QString strSoobchenie){//Вывод сообщения на определённое время в строку состояния.
/////////////////////////////////////////
//---С Т Р О К А   С О С Т О Я Н И Я---//
/////////////////////////////////////////
	slotSS();//Обнуляем, если надо указатель заголовка, и останавливаем таймер.
	if(!strSoobchenie.isEmpty()){//Если строчка не пустая, то...
		m_ptmrSS->start(m_ntSS);//Запускаем таймер
		if(m_plblStatusBar)//Если это не нулевой указатель, то...
			m_plblStatusBar->setText(strSoobchenie);//Отображаем текст в строке состояния
		if(m_psbrStatusBar)//Если это не нулевой указатель, то...
			m_psbrStatusBar->showMessage(strSoobchenie);//Отображаем текст в строке состояния.
	}
}

void DCMessage::qdebug(QString strDebug){//Слот отладки, излучающий строчку в Лог
/////////////////////
//---Q D E B U G---//
/////////////////////
	emit signalDebug(strDebug);//Испускаем сигнал со строчкой Лог
}

void DCMessage::dcinfo(QStringList strlInfo){//Слот информации, излучающий строчку в Лог
/////////////////////
//---D C I N F O---//
/////////////////////
	emit signalInfo(strlInfo);//Испускаем сигнал со строчкой Лог
}

void DCMessage::slotSS(){//Слот очистки строки состояния и остановки таймера.
/////////////////////////////////////////////////////////
//---О Ч И С Т К А   С Т Р О К И   С О С Т О Я Н И Я---//
/////////////////////////////////////////////////////////
	m_ptmrSS->stop();//Останавливаем таймер.
	if(m_plblZagolovokOdin){//Если указатель не нулевой, то...
		m_plblZagolovokOdin->setStyleSheet("color: rgb(0, 0, 0)");//Черный цвет у заголовка по умолчанию.
		m_plblZagolovokOdin = 0;//Обнуляем указатель.
		if(m_plblZagolovokDva){//Если указатель не нулевой у второго заголовка, то...
			m_plblZagolovokDva->setStyleSheet("color: rgb(0, 0, 0)");//Черный цвет второго заголовка по умолч.
			m_plblZagolovokDva = 0;//Обнуляем указатель.
		}
	}
	
	if(m_plblStatusBar)//Если это не нулевой указатель, то...
		m_plblStatusBar->setText("");//Показываем пустую строчку, т.к. произошло прерывание по таймеру.
	if(m_psbrStatusBar)//Если это не нулевой указатель, то...
		m_psbrStatusBar->showMessage("");//Отображаем пустую строчку в строке состояния.
}
