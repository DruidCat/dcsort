#ifndef DCVREMYA_H
#define DCVREMYA_H

#include <QtCore>
#include <QTimer>

class DCVremya : public QThread
{
	Q_OBJECT
public:
	DCVremya();//Конструктор
	~DCVremya();//Деструктор
	void startTimer();//Запуск таймера.
	void stopTimer();//Остановка таймера.

signals:
	void signalTimer();//Произошёл тайм аут по таймеру.

private slots:
	void slotTimer();//Слот испускает сигнал о том, что произошёл тайм аут таймера.

private:
	QTimer* m_ptmrTimer;// Указатель таймера.
	void run();//Запуск потока.
};

#endif // DCVREMYA_H
