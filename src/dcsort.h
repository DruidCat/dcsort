#ifndef DCSORT_H
#define DCSORT_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QTime>

#include "dcmessage.h"
#include "dcvremya.h"

namespace Ui {
class DCSort;
}

class DCSort : public QMainWindow{
	Q_OBJECT

public:
	explicit DCSort(QWidget* proditel = 0);
	~DCSort();

private slots:
	void slotLibPut();//Слот задачи пути к библиотеке.
	void slotSortPut();//Слот задачи пути где будут сортироваться файлы. 
	void slotSortFaili();//Слот начала сортировки файлов.
	void slotSortProvodnik();//Слот открытия папки отсортированных файлов.
	void slotSortOtmena();//Слот отмены процесса сортировки.
	void slotSortVihod();//Слот выхода из программы.
	void slotSortRabota();//Слот отображающий время работы Сортировки.

private:
	Ui::DCSort* ui;
	DCMessage* m_pdcmessage = 0;//Указатель на объект класса сообщений обнуляем.
	DCVremya* m_pdcvremya = 0;//Указатель на поток времени.
	QFile* m_pflCopy = 0;//Указатель на файл со скопированными файлами..
	QTextStream* m_ptstCopy = 0;
	QTime* m_ptmStart = 0;//Время начала сортировки.
	//---реестр---//
	QSettings m_oreestr;//Настройки программы которые будут хранится в системном реестре.
	void ustReestr();//Запись настроек программы
	void polReestr();//Чтение настроек программы
	QString polReestrMaski();//Возвращает настройку маски.
	void ustReestrMaski(QString strNastroikaMaski);//Устанавливает настроики маски.
	QString polReestrFailImya();//Возвращаем настройки имени файла.
	void ustReestrFailImya(QString strReestrFailImya);//Устанавливаем настройки имени файла.
	//---рекурсия---//
	void startSchet(const QDir& dir);//Рекурсивный метод подсчёта копируемых файлов.
	void startSort(const QDir& dir);//Рекурсивный метод  поиска файлов

	void cvetSort();//Цвет граф по умолчанию.
	QString polMasku();//Возвращает маску файлов, которые нужно отсортировать.
	bool udalitFaili();//Удалить файлы, которые были скопированны на сортировку.

	QString m_strSortPut;//Место сортировки файлов.
	bool m_blRekursiya;//Флаг выхода функции из рекурсии.
	int m_ntSchet;//Счётчик количества копируемых файлов.
	int m_ntKolichestvo;//Количество отсортированных файлов файлов.
	//---имя-сортируемого-файла---//
	QString m_strFailImya;//Имя сортируемого файла.
	int m_ntCopy;//Число одинаковых имён копируемых файлов.
	bool m_blFailImya;//Поставлена галочка на Имени сортируемого файла или нет.
	int m_ntFailImya;//Тип отображения имени файла.
	QString m_strFailImyaFormat;//Формат отображения сортируемого файла.
	//---расширения типов файлов---//
	QString m_strIzob;//Перечень расширений изображений.
	QString m_strVideo;//Перечень расширений видео.
	QString m_strMuzika;//Перечень расширений музыки.
	QString m_strDoc;//Перечень расширений документов.
};

#endif // DCSORT_H
