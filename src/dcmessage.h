#ifndef DCMESSAGE_H
#define DCMESSAGE_H

#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QStatusBar>

enum ENMessage {sozdat,izmenit,udalit,registraciya,vhod,parol,vozrast,grafik,nedelya,telefon,strana,shrift};

class DCMessage : public QObject{
	Q_OBJECT
public:
	explicit DCMessage(QLabel* plblStatusBar, int ntVremya, QObject *proditel = 0);//Конструктор.
	explicit DCMessage(QStatusBar* psbrStatusBar, int ntVremya, QObject *proditel = 0);//Конструктор.
	~DCMessage();//Деструктор.
	//Сообщение повляется в окне и в строке состояния.
	void warning(QLabel* plblZagolovok, QString strSoobchenie);//Предупреждение с подсветкой заголовка.
	//Предупреждение с подсветкой двух заголовков.
	void warning(QLabel* plblZagolovokOdin, QLabel* plblZagolovokDva, QString strSoobchenie);
	void warning(QString strZagolovok, QString strSoobchenie);//Предупреждение без подсветки заголовка.
	void information(ENMessage enmes, QString strSoobchenie);//Информация 
	void information(ENMessage enmes, QStringList strlLog, QString strSoobchenie);//Информация в логи
	void critical(QLabel* plblZagolovok, QString strSoobchenie);//Ошибка с подсветкой заголовка.
	void critical(QString strZagolovok, QString strSoobchenie);//Ошибка в виде сообщения.
	void statusbar(QString strSoobchenie);//Вывод сообщения на определённое время в строку состояния.
	
private:
	//---строка-состояния---//
	QStatusBar* m_psbrStatusBar = 0;//Указатель на имя строчки, в которой будет отображаться текст.
	QLabel* m_plblStatusBar = 0;//Указатель на имя строчки, в которой будет отображаться текст.
	QLabel* m_plblZagolovokOdin = 0;//Указатель заголовка, который будет подсвечиваться.
	QLabel* m_plblZagolovokDva = 0;//Указатель второго заголовка, который будет подсвечиваться.
	QTimer* m_ptmrSS = 0;//Таймер на заданное время
	int m_ntSS;//Время отображения сообщения в строке состояния
	//---логи---//
	void qdebug(QString strDebug);//Слот отладки
	void dcinfo(QStringList strlInfo);//Слот Информации
	QString m_strImyaKlassa;//Имя класса, из которого формируется лог ошибки.

signals:
	//---логи---//
	void signalDebug(QString strDebug);//Сигнал, отправляющий строчку отладки в логи
	void signalInfo(QStringList strlInfo);//Сигнал, отправляющий строчку информации в логи

private slots:
	//---строка-состояния---//
	void slotSS();//Слот очистки строки состояния и остановки таймера.
};

#endif // DCMESSAGE_H
