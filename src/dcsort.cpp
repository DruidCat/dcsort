#include "dcsort.h"
#include "ui_dcsort.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

DCSort::DCSort(QWidget* proditel):QMainWindow(proditel), ui(new Ui::DCSort), m_oreestr("DC", "Sort"){//
////////////////////////////////
//---К О Н С Т Р У К Т О-Р----//
////////////////////////////////
	ui->setupUi(this);

	m_pdcmessage = new DCMessage(ui->psbrStrokaSostoyaniya, 30000, this);//Указатель на объект класса сообщени
	m_pflCopy = new QFile;//Создаём динамический указатель на файл со списком скопированнных файлов.
	m_ptstCopy = new QTextStream(m_pflCopy);//Добавляем указатель файла в динамический указатель потока.
	m_pdcvremya = new DCVremya;//Указатель на поток времени.
	m_ptmStart = new QTime;//Создаём динамический указатель на старт сортировки.

	this->setWindowTitle(tr("ДК Сортировщик. Автор: Синебрюхов Сергей. Почта: druidcat@yandex.ru."));
	this->setWindowIcon(QIcon(":/images/logo.ico"));//Устанавливаем иконку на запущеном приложении.

	connect( 	ui->pcmdLibPut,
				SIGNAL(clicked()),
				this,
				SLOT(slotLibPut()));//Клик кнопки задачи пути к библиотеке.

	connect( 	ui->pcmdSortPut,
				SIGNAL(clicked()),
				this,
				SLOT(slotSortPut()));//Клик кнопки задачи пути где будут сортироваться файлы

	connect( 	ui->pcmdSortFaili,
				SIGNAL(clicked()),
				this,
				SLOT(slotSortFaili()));//Клик кнопки начала сортировки файлов.

	connect( 	ui->pcmdSortProvodnik,
				SIGNAL(clicked()),
				this,
				SLOT(slotSortProvodnik()));//Открыть папку, в кототорой отсортированны файлы.

	connect( 	ui->pcmdSortOtmena,
				SIGNAL(clicked()),
				this,
				SLOT(slotSortOtmena()));//Кнопка Отмены процесса сортировки. 

	connect( 	ui->pcmdSortVihod,
				SIGNAL(clicked()),
				this,
				SLOT(slotSortVihod()));//Клик кнопки Выход.

	connect( 	m_pdcvremya,
				SIGNAL(signalTimer()),
				this,
				SLOT(slotSortRabota()));//Время работы сортировщика.

	m_pdcvremya->start();//Запуск потока с таймером.
	m_strSortPut = "";//Путь сортировки файлов по умолчанию.
	m_blRekursiya = true;//Функция в работе.
	m_ntSchet = 0;//количество копируемых файлов.
	m_ntKolichestvo = 0;//Иннициализируем 0 количество отсортированных файлов.
	m_strFailImya = "";//Инициализируем пустой строкой.
	m_ntCopy = 0;//Обнуляем.
	//---расширения типов файлов---//
	m_strIzob = " *.png *.jpg *.jpeg *.jpe *.gif *.psd *.tif *.tiff *.bmp ";
	m_strVideo = " *.avi *.mp4 *.m4v *.mov *.mkv *.mpg *.mpeg";
	m_strMuzika = " *.mp3 *.m4a *.wav *.ogg *.flac *.aac";
	m_strDoc = " *.pdf *.doc *.docx *.xls *.xlsx *.txt *.zip *.rar *.7z *.rtf *.odt *.fb2 *.djvu";
	//---устанавливаем описания расширений---//
	ui->pcbxIzob->setToolTip(m_strIzob);//на флажке изображения появляется описание расширений файлов.
	ui->pcbxVideo->setToolTip(m_strVideo);//на флажке видео появляется описание расширений файлов.
	ui->pcbxMuzika->setToolTip(m_strMuzika);//на флажке музыки появляется описание расширений файлов.
	ui->pcbxDoc->setToolTip(m_strDoc);//на флажке документов появляется описание расширений файлов.
	//---задаём имя файлов---//
	ui->pgbxFile->setCheckable(true);//Делаем groupbox выбираемым с помощью галочки
	ui->pgbxFile->setChecked(false);//Убираем галочку с groupbox.
	ui->pgbxFile->setToolTip(tr("Если не выбирать данный флажок, то имена файлов останутся без изменений"));

	QStringList lstFailData = (QStringList()<<tr("пусто")<<tr("год")<<tr("месяц")<<tr("день")<<tr("-")
			<<tr("пробел"));
	ui->pcbxFail_1->clear();
	ui->pcbxFail_1->addItems(lstFailData);
	ui->pcbxFail_2->clear();
	ui->pcbxFail_2->addItems(lstFailData);
	ui->pcbxFail_3->clear();
	ui->pcbxFail_3->addItems(lstFailData);
	ui->pcbxFail_4->clear();
	ui->pcbxFail_4->addItems(lstFailData);
	ui->pcbxFail_5->clear();
	ui->pcbxFail_5->addItems(lstFailData);
	ui->pcbxFail_6->clear();
	ui->pcbxFail_6->addItems(lstFailData);

	QStringList lstFailImya=(QStringList()<<tr("имя файла")<<tr("время создания файла")<<tr("сумма секунд"));
	ui->pcbxFail_7->clear();
	ui->pcbxFail_7->addItems(lstFailImya);

	m_blFailImya = false;//галочка не поставлена на Имени сортируемого файла.
	m_ntFailImya = 0;//Тип отображения имени файла.
	m_strFailImyaFormat = "";//Пустой формат отображения сортируемого файла.
	polReestr();//Чтение настроек программы
}

DCSort::~DCSort(){//Деструктор
//////////////////////////////
//---Д Е С Т Р У К Т О-Р----//
//////////////////////////////
	ustReestr();//При закрытии программы записываем настройки программы
	delete m_pdcmessage;//Обязательно удаляем указатель объекта методов сообщений.
	m_pdcmessage = 0;//Обнуляем указатель.
	if(m_pflCopy->exists())//Если файл в папке есть, то...
		m_pflCopy->remove();//Удаляем файл.
	delete m_pflCopy;//Обязательно удаляем указатель объекта методов файла со списком скопированных файлов.
	m_pflCopy= 0;//Обнуляем указатель.
	delete m_ptstCopy;//Удаляем указать.
	m_ptstCopy = 0;//Обнуляем указатель.
	m_pdcvremya->exit(0);//Удаляем поток.
	m_pdcvremya->deleteLater();//Удалить позже, когда программа полностью закроется.
	m_pdcvremya = 0;//Обнуляем указатель на поток.
	delete m_ptmStart;//Удаляем указатель.
	m_ptmStart = 0;//Обнуляем указатель.
	delete ui;
}

void DCSort::ustReestr(){//Запись настроек программы
///////////////////////////////////////
//---З А П И С Ь   Н А С Т Р О Е К---//
///////////////////////////////////////
	m_oreestr.beginGroup("/Nastroiki");//Открываем группу /Настройки
		m_oreestr.setValue("/sort_shirina_okna", width());//Записываем ширину окна
		m_oreestr.setValue("/sort_visota_okna", height());//Записываем высоту окна
		m_oreestr.setValue("/sort_put_lib", ui->pldtLibPut->text());//Записываем
		m_oreestr.setValue("/sort_put_sort", ui->pldtSortPut->text());//Записываем
		m_oreestr.setValue("/sort_maska", polReestrMaski());//Записываем
		m_oreestr.setValue("/sort_fail_imya", polReestrFailImya());//Записываем
	m_oreestr.endGroup();//Закрываем группу /Настройки
}

void DCSort::polReestr(){//Чтение настроек программы
///////////////////////////////////////
//---Ч Т Е Н И Е   Н А С Т Р О Е К---//
///////////////////////////////////////
	m_oreestr.beginGroup("/Nastroiki");//Открываем группу /Настройки
		int ntShirinaOkna = m_oreestr.value("/sort_shirina_okna", width()).toInt();//Читаем ширину окна
		int ntVisotaOkna = m_oreestr.value("/sort_visota_okna", height()).toInt();//Читаем высоту окна
		QString strLibPut = m_oreestr.value("/sort_put_lib", "").toString();//Читаем путь к библиотеке. 
		QString strSortPut = m_oreestr.value("/sort_put_sort", "").toString();//Читаем путь к сортировке. 
		QString strSortFaili = m_oreestr.value("/sort_maska", "1,0,0,0").toString();//Читаем маску.
		QString strFailImya = m_oreestr.value("/sort_fail_imya", "0,0,0,0,0,0,0,0").toString();//Чит имя фаила
	m_oreestr.endGroup();//Закрываем группу /Настройки

	resize(ntShirinaOkna, ntVisotaOkna);//Выставляем размеры окна
	ui->pldtLibPut->setText(strLibPut);//Вставляем путь к библиотеке из реестра
	ui->pldtSortPut->setText(strSortPut);//Вставляем путь к месту сортировки 
	ustReestrMaski(strSortFaili);//Вставляем маску в строку.
	ustReestrFailImya(strFailImya);//Выставляем параметры имени сортируемого файла из реестра.
}

QString DCSort::polReestrMaski(){//Возвращает настройку маски.
/////////////////////////////////////////////////////////
//---П О Л У Ч И Т Ь   Н А С Т Р О Й К И   М А С К И---//
/////////////////////////////////////////////////////////
	QString strNastroikaMaski("");
	if(ui->pcbxIzob->isChecked())
		strNastroikaMaski += "1,";
	else
		strNastroikaMaski += "0,";

	if(ui->pcbxVideo->isChecked())
		strNastroikaMaski += "1,";
	else
		strNastroikaMaski += "0,";

	if(ui->pcbxMuzika->isChecked())
		strNastroikaMaski += "1,";
	else
		strNastroikaMaski += "0,";

	if(ui->pcbxDoc->isChecked())
		strNastroikaMaski += "1";
	else
		strNastroikaMaski += "0";

	return strNastroikaMaski;
}

void DCSort::ustReestrMaski(QString strNastroikaMaski){//Устанавливает настроики маски.
/////////////////////////////////////////////////////////////
//---У С Т А Н О В И Т Ь   Н А С Т Р О Й К И   М А С К И---//
/////////////////////////////////////////////////////////////
	QStringList slsNastroikaMaski = strNastroikaMaski.split(",");//Разделяем строку на цифры через запятую
	if(slsNastroikaMaski.size() != 4)//Если в настройне не 4 элемента, то..
		return;//Ошибка, выход.

	if(slsNastroikaMaski[0] == "1")
		ui->pcbxIzob->setChecked(true);
	else
		ui->pcbxIzob->setChecked(false);

	if(slsNastroikaMaski[1] == "1")
		ui->pcbxVideo->setChecked(true);
	else
		ui->pcbxVideo->setChecked(false);

	if(slsNastroikaMaski[2] == "1")
		ui->pcbxMuzika->setChecked(true);
	else
		ui->pcbxMuzika->setChecked(false);

	if(slsNastroikaMaski[3] == "1")
		ui->pcbxDoc->setChecked(true);
	else
		ui->pcbxDoc->setChecked(false);
}

QString DCSort::polReestrFailImya(){//Возвращаем настройки имени файла.
/////////////////////////////////////////////////////////////////////
//---П О Л У Ч И Т Ь   Н А С Т Р О Й К И   И М Е Н И   Ф А Й Л А---//
/////////////////////////////////////////////////////////////////////
	QString strReestrFailImya("");//Накопительная строка настроек имени сортируемого файла.

	if(ui->pgbxFile->isChecked())//Если выбрана галочка на имени сортируемого файла, то...
		strReestrFailImya += "1,";
	else//в противном случае...
		strReestrFailImya += "0,";

	strReestrFailImya += QString::number(ui->pcbxFail_1->currentIndex()) + ",";//записываем выбор в реестр.
	strReestrFailImya += QString::number(ui->pcbxFail_2->currentIndex()) + ",";//записываем выбор в реестр.
	strReestrFailImya += QString::number(ui->pcbxFail_3->currentIndex()) + ",";//записываем выбор в реестр.
	strReestrFailImya += QString::number(ui->pcbxFail_4->currentIndex()) + ",";//записываем выбор в реестр.
	strReestrFailImya += QString::number(ui->pcbxFail_5->currentIndex()) + ",";//записываем выбор в реестр.
	strReestrFailImya += QString::number(ui->pcbxFail_6->currentIndex()) + ",";//записываем выбор в реестр.
	strReestrFailImya += QString::number(ui->pcbxFail_7->currentIndex());//записываем выбор в реестр.

	return strReestrFailImya;//Возращаем строчку настроек реестра касаемых имени сортируемого файла.
}

void DCSort::ustReestrFailImya(QString strReestrFailImya){//Устанавливаем настройки имени файла.
/////////////////////////////////////////////////////////////////////////
//---У С Т А Н О В И Т Ь   Н А С Т Р О Й К И   И М Е Н И   Ф А Й Л А---//
/////////////////////////////////////////////////////////////////////////
	QStringList slsReestrFailImya = strReestrFailImya.split(",");//Разделяем строку на цифры через запятую
	if(slsReestrFailImya.size() != 8)//Если в настройне не 8 элемента, то..
		return;//Ошибка, выход.

	if(slsReestrFailImya[0] == "1")//Если 1,то... 
		ui->pgbxFile->setChecked(true);//Ставим залочку на Имя сортируемого файла
	else//в противном случае.
		ui->pgbxFile->setChecked(false);//Убираем галочку на Имени сортируемого файла.

	ui->pcbxFail_1->setCurrentIndex(slsReestrFailImya[1].toInt());
	ui->pcbxFail_2->setCurrentIndex(slsReestrFailImya[2].toInt());
	ui->pcbxFail_3->setCurrentIndex(slsReestrFailImya[3].toInt());
	ui->pcbxFail_4->setCurrentIndex(slsReestrFailImya[4].toInt());
	ui->pcbxFail_5->setCurrentIndex(slsReestrFailImya[5].toInt());
	ui->pcbxFail_6->setCurrentIndex(slsReestrFailImya[6].toInt());
	ui->pcbxFail_7->setCurrentIndex(slsReestrFailImya[7].toInt());
}

void DCSort::startSchet(const QDir& dir){//Рекурсивный метод подсчёта копируемых файлов.
///////////////////////////////////////////
//---Р Е К У Р С И В Н Ы Й   П О И С К---//
///////////////////////////////////////////
	QApplication::processEvents();//Вызов этого метода нужен, чтобы программа не зависала при вызове рекурсии.

	if(m_blRekursiya){//Если добро на работу, то...
		m_ntSchet = m_ntSchet + dir.entryList(polMasku().split(" "), QDir::Files).size();//Счёт
		QStringList listDir = dir.entryList(QDir::Dirs);
		foreach (QString subdir, listDir) { 
			if (subdir == "." || subdir == "..") { 
				continue; 
			} 
			startSchet(QDir(dir.absoluteFilePath(subdir))); 
		}
	}
	else{//Если отмена работы рекурсии, то...
		return;//Выходим из рекурсивного метода.
	}
}

void DCSort::startSort(const QDir& dir){//Рекурсивный метод поиска файлов
///////////////////////////////////////////
//---Р Е К У Р С И В Н Ы Й   П О И С К---//
///////////////////////////////////////////
	QApplication::processEvents();//Вызов этого метода нужен, чтобы программа не зависала при вызове рекурсии.

	if(m_blRekursiya){//Если добро на работу, то...
		QStringList slsPut = dir.entryList(polMasku().split(" "), QDir::Files);//Маска
	 
		foreach (QString strFile, slsPut){//Цикл показа файлов с рекурсивной функцией.
			if(!m_blRekursiya)//Если произошла отмена работы сортировки, то...
				return;//Выходим из рекурсии
			QFile flSort (dir.absoluteFilePath(strFile));//Сортируемый файл.
			QFileInfo fnfSort(dir.absoluteFilePath(strFile));//Информация по сортируемому файлу.
			QString strImyaSortFaila = fnfSort.completeBaseName();//Полное имя сортируемого файла.
			QString strGod("");//Переменная будет содержать год создания файла.
			QString strSortData("");//Переменная будет содержать дату создания сортируемого файла.
			QDate dtImyaSortFaila;//Дата сортируемого файла, если имя файла это дата.
			QTime tmImyaSortFaila;//Время сортируемого файла, если в имени файла второй кусок время
			bool blImyaSortFailaData(false);//Имя сортированного файла не дата.
			bool blImyaSortFailaVremya(false);//В Имени сортированного файла второй кусок не время.
			QStringList slsImyaSortFaila = strImyaSortFaila.split(" ");//Разделяем по " "
			if(slsImyaSortFaila.size() >=2){//Если есть два и более куска разделённых " ", то...
				QStringList slsDataSortFaila = slsImyaSortFaila[0].split("-");//Разделяем по "-"
				QStringList slsVremyaSortFaila = slsImyaSortFaila[1].split("-");//Разделяем по "-"
				if(slsDataSortFaila.size() == 3){//Если 3 куска разделённых "-", то...
					if(dtImyaSortFaila.setDate(slsDataSortFaila[0].toInt(),slsDataSortFaila[1].toInt(),
							slsDataSortFaila[2].toInt())){//Если эта дата существует, то...
						strGod = slsDataSortFaila[0];//Это год.
						strSortData = dtImyaSortFaila.toString("yyyy-MM-dd");//Это дата создания файла.
						blImyaSortFailaData = true;//Имя сортируемого файла - это Дата.
						if(slsVremyaSortFaila.size() == 3){//Если там три куска, то...
							if(tmImyaSortFaila.setHMS(slsVremyaSortFaila[0].toInt(),
									slsVremyaSortFaila[1].toInt(),slsVremyaSortFaila[2].toInt())){//Если время
								blImyaSortFailaVremya = true;//В имени сорт файла второй кусок это время.
							}
						}
					}
				}
			}

			if(!blImyaSortFailaData){//Если имя сортированного файла не дата, то...
				strGod = fnfSort.created().date().toString("yyyy");//Год создания файла.
				strSortData = fnfSort.created().date().toString("yyyy-MM-dd");//папки с датами.
			}
			QDir drSort(m_strSortPut);//Каталог сортировки файлов.
			QDir drSortGod(m_strSortPut+QDir::separator()+strGod);//Каталог Года сортировки
			QDir drSortData(drSortGod.absolutePath() + QDir::separator()+ strSortData);//Папка с именем Даты

			bool blSortGod = true;//Ошибка создания папки Год.
			bool blSortData = true;//Ошибка создания папки Дата.
			if(!flSort.isOpen()){//Если файл не открыт, то...
				if(!drSortGod.exists()){//Если нет папки Год, то...
					if(!drSort.mkdir(strGod)){//Если не создался каталог года, то...
						QMessageBox msgBox(this);//Создаем объект Сообщения
						msgBox.setWindowTitle(tr("Ошибка:"));//Устанавливаем заголовок окна сообщения.
						msgBox.setIcon(QMessageBox::Question);
						msgBox.setText(tr("Пропустить создание папки: ")+strGod+"?");//Добав. текст
						msgBox.setInformativeText(tr("Пропустить?"));//Добавляем вопрос в сообщение
						//Создаем кнопку Ок, и добавляем ее в сообщение
						QPushButton* pcmdOk = msgBox.addButton(QMessageBox::Ok);
						//Создаем кнопку Отмена и добавляем ее в сообщение
						QPushButton* pcmdChancel = msgBox.addButton(tr("Отмена"), QMessageBox::ActionRole);
						msgBox.setDefaultButton(QMessageBox::Ok);//Делаем по умолчанию выделенную кнопку Ок
						msgBox.exec();//Запускаем Сообщение
						if(msgBox.clickedButton() == pcmdChancel){//Если нажата кнопка Отмена, то...
							blSortGod = false;//Ошибка создания папки Год.	
							m_blRekursiya = false;//Выход из поиска файлов.
						}
						if (msgBox.clickedButton() == pcmdOk){//Если нажата кнопка Ок, то...
							blSortGod = false;//Ошибка создания папки Год.	
						}
					}
				}
				if(blSortGod){//Если не было ошибок по созданию папки Год, то...
					if(!drSortData.exists()){//Если папки Дата не существует, то...
						if(!drSortGod.mkdir(strSortData)){//Если папка Дата не создалась, то...
							QMessageBox msgBox(this);//Создаем объект Сообщения
							msgBox.setWindowTitle(tr("Ошибка:"));//Устанавливаем заголовок окна сообщения.
							msgBox.setIcon(QMessageBox::Question);
							msgBox.setText(tr("Пропустить создание папки: ")+strSortData+"?");//Добав. текст
							msgBox.setInformativeText(tr("Пропустить?"));//Добавляем вопрос в сообщение
							//Создаем кнопку Ок, и добавляем ее в сообщение
							QPushButton* pcmdOk = msgBox.addButton(QMessageBox::Ok);
							//Создаем кнопку Отмена и добавляем ее в сообщение
							QPushButton* pcmdChancel = msgBox.addButton(tr("Отмена"),QMessageBox::ActionRole);
							msgBox.setDefaultButton(QMessageBox::Ok);//Делаем по умолчанию выделенную кнопк Ок
							msgBox.exec();//Запускаем Сообщение
							if(msgBox.clickedButton() == pcmdChancel){//Если нажата кнопка Отмена, то...
								blSortData = false;//Ошибка создания папки Даты.
								m_blRekursiya = false;//Выход из поиска файлов.
							}
							if (msgBox.clickedButton() == pcmdOk){//Если нажата кнопка Ок, то...
								blSortData = false;//Ошибка создания папки Даты.
							}
						}	
					}
				}

				if(blSortData){//Если нет ошибок при создании папки Дата, то...
					QString strFailImya("");//Строка, которая будет содержать имя сортируемого файла.

					if(m_blFailImya){//Если галочка стоит в Имени сортировки файлов, то...
						if(m_ntFailImya == 0){//Если оставить имя файла, то...
							if(blImyaSortFailaData){//Если имя файла - это дата, то...
								strFailImya = dtImyaSortFaila.toString(m_strFailImyaFormat);//Дата создания
								for(int ntShag = 1; ntShag < slsImyaSortFaila.size(); ++ntShag){//Куски собира
									strFailImya = strFailImya + slsImyaSortFaila[ntShag];//Прибавляем кусок
									if(ntShag != (slsImyaSortFaila.size()-1))//Если не последний кусок, то...
										strFailImya = strFailImya + " ";//Добавляем пробел.
								}
							}
							else{//в противном случае...
								strFailImya = fnfSort.created().date().toString(m_strFailImyaFormat)
										+fnfSort.completeBaseName();//Формат + имя файла без изменений.
							}
						}
						else{
							if(m_ntFailImya == 1){//Если это время создания, то...
								if(blImyaSortFailaData){//Если имя файла - это дата, то...
									strFailImya = dtImyaSortFaila.toString(m_strFailImyaFormat);//Дата в имени
									if(blImyaSortFailaVremya)//Если в имени файла есть время создания, то...
										strFailImya=strFailImya+tmImyaSortFaila.toString("hh-mm-ss");
									else//В противном случае...
										strFailImya=strFailImya+fnfSort.created().time().toString("hh-mm-ss");
								}
								else{//в противном случае...
									strFailImya = fnfSort.created().date().toString(m_strFailImyaFormat)
											+fnfSort.created().time().toString("hh-mm-ss");//Формат+время созд
								}
							}
							else{//В противном случае...
								int ntSec(0);//Сумма секунд от создания файла.
								if(blImyaSortFailaData){//Если имя файла - это дата, то...
									strFailImya = dtImyaSortFaila.toString(m_strFailImyaFormat);//Дата содания
									if(blImyaSortFailaVremya){//Если в имени файла есть время создания файла
										ntSec =	tmImyaSortFaila.hour()*3600
												+tmImyaSortFaila.minute()*60
												+tmImyaSortFaila.second();//Считаем кол-во сек от создания
									}
									else{//В противном случае...
										ntSec =	fnfSort.created().time().hour()*3600
												+fnfSort.created().time().minute()*60
												+fnfSort.created().time().second();//Считаем кол-во сек от соз
									}
									strFailImya = strFailImya + QString::number(ntSec);//Формат+сумма сек
								}
								else{//в противном случае...
									ntSec =	fnfSort.created().time().hour()*3600
											+fnfSort.created().time().minute()*60
											+fnfSort.created().time().second();//Считаем кол-во сек от соз
									strFailImya = fnfSort.created().date().toString(m_strFailImyaFormat)
											+QString::number(ntSec);//Формат + сумма секунд от создания файла.
								}
							}
						}
					}
					else//Если галочка не стоит, то оставляем имя сортируемого файла без изменения.
						strFailImya = fnfSort.completeBaseName();

					if(strFailImya != m_strFailImya)//Если эти переменные не равны, то это файл с неодинаковым
						m_ntCopy = 0;//Обнуляем счётчик одинаковых файлов по времени.
					m_strFailImya = strFailImya;//Приравниваем параметры.
					QFile flCopy(drSortData.absolutePath()+QDir::separator()+strFailImya+"."
							+fnfSort.suffix());//Файл с новым именем по секундам в месте его копирования.
					for(int ntShag = 0; ntShag<100; ++ntShag){//Цикл из 100 одинаковых файлов.
						if(flCopy.exists()){//Если файл с таким именем существует, то...
							QString strCopyCopy = "("+QString::number(++m_ntCopy)+")";//Создаём файл со ()
							QFile flCopyCopy(drSortData.absolutePath()+QDir::separator()+strFailImya
									+strCopyCopy+"."+fnfSort.suffix());//Файл со ()
							if(!flCopyCopy.exists())//Если файла со скобками и цифрой не существует, то...
								break;//Выход из перечисления.
						}
						else{//Если файла с таким имененем не существует, то...
							m_ntCopy = 0;//Обнуляем счётчик файлов с одинаковым именем.
							break;//Выход из перечисления.
						}
					}
					QString strCopy("");//Строка со ()
					if(m_ntCopy)//Если счётчик не 0, то...
						strCopy = "("+QString::number(m_ntCopy)+")";//Добавляем номер в скобки одинакого файла

					if(flSort.copy(dir.absoluteFilePath(strFile), drSortData.absolutePath()+QDir::separator()
							+strFailImya+strCopy+"."+fnfSort.suffix())){//Если файл скопировался,то
						*m_ptstCopy<< dir.absoluteFilePath(strFile) + "\r\n";//Добавляем в поток
						ui->plcdSortKolichestvo->display(++m_ntKolichestvo);//+1 к сортированному файлу, показ
						ui->ppbrSort->setValue(m_ntKolichestvo);//Отобразить на панели прогресса.
						ui->ptdtSort->append(dir.absoluteFilePath(strFile));//Отображаем файлы
						slotSortRabota();//Слот отображает время сортировки.
					}
					else{
						QMessageBox msgBox(this);//Создаем объект Сообщения
						msgBox.setWindowTitle(tr("Ошибка:"));//Устанавливаем заголовок окна сообщения.
						msgBox.setIcon(QMessageBox::Question);
						msgBox.setText(tr("Пропустить копирование файла: ")+fnfSort.absoluteFilePath()+"?");//
						msgBox.setInformativeText(tr("Пропустить?"));//Добавляем вопрос в сообщение
						//Создаем кнопку Ок, и добавляем ее в сообщение
						QPushButton* pcmdOk = msgBox.addButton(QMessageBox::Ok);
						//Создаем кнопку Отмена и добавляем ее в сообщение
						QPushButton* pcmdChancel = msgBox.addButton(tr("Отмена"),QMessageBox::ActionRole);
						msgBox.setDefaultButton(QMessageBox::Ok);//Делаем по умолчанию выделенную кнопк Ок
						msgBox.exec();//Запускаем Сообщение
						if(msgBox.clickedButton() == pcmdChancel){//Если нажата кнопка Отмена, то...
							m_blRekursiya = false;//Выход из поиска файлов.
						}
						if (msgBox.clickedButton() == pcmdOk){//Если нажата кнопка Ок, то...
						}
					}
				}
			}
			else{//В противном случае...
				QMessageBox msgBox(this);//Создаем объект Сообщения
				msgBox.setWindowTitle(tr("Ошибка:"));//Устанавливаем заголовок окна сообщения.
				msgBox.setIcon(QMessageBox::Question);
				msgBox.setText(tr("Пропустить копирование файла: ")+fnfSort.absoluteFilePath()+"?");//Добав. 
				msgBox.setInformativeText(tr("Пропустить?"));//Добавляем вопрос в сообщение
				//Создаем кнопку Ок, и добавляем ее в сообщение
				QPushButton* pcmdOk = msgBox.addButton(QMessageBox::Ok);
				//Создаем кнопку Отмена и добавляем ее в сообщение
				QPushButton* pcmdChancel = msgBox.addButton(tr("Отмена"), QMessageBox::ActionRole);
				msgBox.setDefaultButton(QMessageBox::Ok);//Делаем по умолчанию выделенную кнопку Ок
				msgBox.exec();//Запускаем Сообщение
				if(msgBox.clickedButton() == pcmdChancel){//Если нажата кнопка Отмена, то...
					m_blRekursiya = false;//Выход из поиска файлов.
				}
				if (msgBox.clickedButton() == pcmdOk){//Если нажата кнопка Ок, то...
				}
			}
		} 
 
		QStringList listDir = dir.entryList(QDir::Dirs);
		foreach (QString subdir, listDir) { 
			if (subdir == "." || subdir == "..") { 
				continue; 
			} 
			startSort(QDir(dir.absoluteFilePath(subdir))); 
		}
	}
	else{//Если отмена работы рекурсии, то...
		return;//Выходим из рекурсивного метода.
	}
}

void DCSort::cvetSort(){//Цвет граф по умолчанию.
/////////////////////////////////////////////
//---Ц В Е Т А   П О   У М О Л Ч А Н И Ю---//
/////////////////////////////////////////////
	ui->plblLibPut->setStyleSheet("color: rgb(0, 0, 0)");
	ui->plblSortPut->setStyleSheet("color: rgb(0, 0, 0)");
	ui->plblSortFaili->setStyleSheet("color: rgb(0, 0, 0)");
}

QString DCSort::polMasku(){//Возвращает маску файлов, которые нужно отсортировать.
///////////////////////////////////////////////////////////
//---П О Л У Ч И Т Ь   М А С К У   С О Р Т И Р О В К И---//
///////////////////////////////////////////////////////////
	QString strMaska("");
	if(ui->pcbxIzob->isChecked())
		strMaska += m_strIzob;
	
	if(ui->pcbxVideo->isChecked())
		strMaska += m_strVideo;
	
	if(ui->pcbxMuzika->isChecked())
		strMaska += m_strMuzika;
	
	if(ui->pcbxDoc->isChecked())
		strMaska += m_strDoc;

	return strMaska;
}

bool DCSort::udalitFaili(){//Удалить файлы, которые были скопированны на сортировку.
///////////////////////////////////////////////////////////////////
//---У Д А Л Е Н И Е   С К О П И Р О В А Н Н Ы Х   Ф А Й Л О В---//
///////////////////////////////////////////////////////////////////
	QDir drProg = QDir::current();//Создаём объект который будет содержать путь программы.
	QString strProgram = drProg.path() + "/copy.txt";//Путь с именем файла Копирования.
	QFile flCopy(strProgram);//Создаём объект на файл copy.txt в котором записан список удаляемых файлов.
	QFile* pflUdalit = new QFile;//Создаём динамический указатель на удаляемые файлы.
	if(flCopy.open(QIODevice::ReadOnly)){//Если файл открылся на чтение, то...
		QTextStream tstPotok(&flCopy);//Добавляем файл copy.txt в поток для чтения.
		QString strUdalit("");//Строчка, в которой будет хранится считаный путь на удаление файла.
		while(!tstPotok.atEnd()){//Запускаем цикл построчного чтения файла, до его полногоп прочтения.
			strUdalit = tstPotok.readLine();//Считываем из потока строчку.
			if(!strUdalit.isEmpty()){//если строка не пустая, то...
				pflUdalit->setFileName(strUdalit);//Именуем указатель на файл удаления.
				if(pflUdalit->exists()){//Если файл в папке есть, то...
					if(!pflUdalit->remove()){//Удаляем файл. Если он не удалился, то...
						QMessageBox msgBox(this);//Создаем объект Сообщения
						msgBox.setWindowTitle(tr("Ошибка:"));//Устанавливаем заголовок окна сообщения.
						msgBox.setIcon(QMessageBox::Question);
						msgBox.setText(tr("При удалении файла: ") + strUdalit
								+ tr(" произошла ошибка. Удалите его вручную. Продолжить удаление файлов?"));
						msgBox.setInformativeText(tr("Продолжить?"));//Добавляем вопрос в сообщение
						//Создаем кнопку Ок, и добавляем ее в сообщение
						QPushButton* pcmdOk = msgBox.addButton(QMessageBox::Ok);
						//Создаем кнопку Отмена и добавляем ее в сообщение
						QPushButton* pcmdChancel = msgBox.addButton(tr("Отмена"), QMessageBox::ActionRole);
						msgBox.setDefaultButton(QMessageBox::Ok);//Делаем по умолчанию выделенную кнопку Ок
						msgBox.exec();//Запускаем Сообщение
						if(msgBox.clickedButton() == pcmdChancel){//Если нажата кнопка Отмена, то...
			   				flCopy.close();//Закрываем файл на чтение.
							delete pflUdalit;//Удаляем указатель удаляемого файла.
							pflUdalit = 0;//Обнуляем указатель удаляемого файла.
							return false;//Выходим из слота.
						}
						if (msgBox.clickedButton() == pcmdOk){//Если нажата кнопка Ок, то...
						}
					}
				}
			}
		}
		if(tstPotok.status() != QTextStream::Ok){//Если статус чтения из потока не Ок, то...
			m_pdcmessage->critical(tr("Ошибка:"),
					tr("Ошибка чтения списка удаляемых файлов, удалите скопированные файлы вручную!"));
   			flCopy.close();//Закрываем файл на чтение.
			delete pflUdalit;//Удаляем указатель удаляемого файла.
			pflUdalit = 0;//Обнуляем указатель удаляемого файла.
			return false;//Выходим из слота.
    	} 
   		flCopy.close();//Закрываем файл на чтение.
	}
	delete pflUdalit;//Удаляем указатель удаляемого файла.
	pflUdalit = 0;//Обнуляем указатель удаляемого файла.
	return true;//Выходим из слота с успехом.
}

void DCSort::slotLibPut(){//Слот задачи пути к библиотеке
/////////////////////////////////////////////////////
//---С Л О Т   П У Т И   К   Б И Б Л И О Т Е К Е---//
/////////////////////////////////////////////////////
	QString strLibPut = QFileDialog::getExistingDirectory(0, "Путь директории файлов.", "");
	if(!strLibPut.isEmpty()){//Если строчка не пустая или не нажата кнопка отмена, то...
		ui->pldtLibPut->clear();//Очищаем строчку от старого пути.
		ui->pldtLibPut->setText(strLibPut);//Вставляем выбранный путь к библиотеке.
	}
}

void DCSort::slotSortPut(){//Слот задачи пути где будут сортироваться файлы.
/////////////////////////////////////////////////////////////
//---С Л О Т   П У Т И   М Е С Т А   С О Р Т И Р О В К И---//
/////////////////////////////////////////////////////////////
	QString strSortPut = QFileDialog::getExistingDirectory(0, "Путь директории сортировки.", "");
	if(!strSortPut.isEmpty()){
		ui->pldtSortPut->clear();//Очищаем строчку от старого пути.
		ui->pldtSortPut->setText(strSortPut);//Вставляем выбранный путь сортировки файлов.
	}
}

void DCSort::slotSortFaili(){//Слот начала сортировки файлов.
/////////////////////////////////////////////////////
//---С Л О Т   Н А Ч А Л А   С О Р Т И Р О В К И---//
/////////////////////////////////////////////////////
	cvetSort();//Цвет граф по умолчанию.
	m_pdcmessage->statusbar("");//Удаляем все сообщения в строке состояния.
	
	QString strLibPut = ui->pldtLibPut->text();//Путь к библиотеке.
	QString strSortPut = ui->pldtSortPut->text();//Путь к месту сортировки.

	if(polMasku().isEmpty()){//Если Маска пустая, то...
		m_pdcmessage->critical(ui->plblSortFaili,
			tr("Пустое поле: ") + ui->plblSortFaili->text() + tr(", выберите элементы сортировки!"));
		return;//Выход из слота
	}
	else{
		QDir drLibPut(strLibPut);//Папка сортируемых файлов
		if(!drLibPut.exists()){//Если данная папка не существует, то...
			m_pdcmessage->critical(ui->plblLibPut,
				tr("Данной папки не существует: ") + ui->plblLibPut->text()
				+ tr(" , укажите существующую папку!"));
			return;//Выход из слота
		}
		else{
			if(strLibPut.isEmpty()){//Если путь к библиотеке пуст, то...
				m_pdcmessage->critical(ui->plblLibPut,
						tr("Пустое поле: ") + ui->plblLibPut->text() + tr(", заполните его!"));
				return;//Выход из слота
			}
			else{
				QDir drSortPut(strSortPut);//Папка места сортировки файлов
				if(!drSortPut.exists()){//Если данная папка не существует, то...
					m_pdcmessage->critical(ui->plblSortPut,
						tr("Данной папки не существует: ") + ui->plblSortPut->text()
						+ tr(" , укажите существующую папку!"));
					return;//Выход из слота
				}
				else{
					if(strSortPut.isEmpty()){//Если путь места сортировки пуст, то...
						m_pdcmessage->critical(ui->plblSortPut,
								tr("Пустое поле: ") + ui->plblSortPut->text() + tr(", заполните его!"));
						return;//Выход из слота
					}
				}
			}
		}
	}

	int ntLibPut = strLibPut.size();//Длина пути библиотеки
	int ntSortPut = strSortPut.size();//Длина пути места сортировки.
	bool blSortPut = true;//Пути взаимовложенные.
	if(ntLibPut >= ntSortPut){//Если путь библиотеки будет больше или равен пути сортировки
		for(int ntShag = 0; ntShag<ntSortPut; ++ntShag){//сравниваем путь библиотеки и сортировки по сортиров.
			if(strSortPut[ntShag] != strLibPut[ntShag]){//Если символы не одинаковые, то...
				blSortPut = false;//Нет совпадения. Пути не взаимовложенные.
				break;//Выходим из перечисления.
			}
		}
	}
	else//в противном случае...
		blSortPut = false;//Пути не взаимовложенные.

	if(blSortPut){//Если не было изменения флага, то пути взаимовложенные.
		m_pdcmessage->warning(ui->plblLibPut, ui->plblSortPut, tr("Путь: ") + strLibPut +
				tr(" вложен в путь: ") + strSortPut + tr(". Это ограничение программы, перенесите ") +
				ui->plblLibPut->text() + tr(" в другое место и повторите попытку!"));
	}

	QDir odrProg = QDir::current();//Создаём объект который будет содержать путь программы.
	QString strProgram = odrProg.path() + "/copy.txt";//Путь с именем файла Копирования.
	m_pflCopy->setFileName(strProgram);//Создаём имя на указатель на файл Копирования.
	if(m_pflCopy->exists()){//Если файл в папке есть, то...
		if(!m_pflCopy->remove()){//Удаляем файл. Если он не удалился, то...
			m_pdcmessage->critical(tr("Ошибка:"), tr("При удалении файла: ") + strProgram 
					+ tr(" произошла ошибка. Выберете другую директорию."));
			return;//Выходим из слота.
		}
	}

	ui->pgbxSort->setEnabled(false);//Деактивируем меню.
	ui->ptdtSort->clear();//Очищаем окно отображения.
	ui->plblSortRabotaTimer->setText("00:00:00");//Отображаем время по нулям.
	m_pdcvremya->startTimer();//Запускаем таймер с частотой обновления секунду.
	*m_ptmStart = QTime::currentTime();//Текущее время
	if(m_pflCopy->open(QIODevice::WriteOnly)){//Если файл открылся для записи, то...
		m_blFailImya = ui->pgbxFile->isChecked();//Присваиваем значение выбранной галочки.
		m_ntFailImya = ui->pcbxFail_7->currentIndex();//Тип отображения имени файла.

		int ntMassFailImya[6]= {
			ui->pcbxFail_1->currentIndex(),
			ui->pcbxFail_2->currentIndex(),
			ui->pcbxFail_3->currentIndex(),
			ui->pcbxFail_4->currentIndex(),
			ui->pcbxFail_5->currentIndex(),
			ui->pcbxFail_6->currentIndex(),
		};//Создаём массив с данными формата имени сортируемого файла

		m_strFailImyaFormat = "";//Обнуляем формат.
		for(int ntShag = 0; ntShag<6; ++ntShag){//Перечисляем первые 6 данных формата имени сортируемого файла
			if(ntMassFailImya[ntShag] == 1)//Если это год, то...
				m_strFailImyaFormat += "yyyy";
			else{
				if(ntMassFailImya[ntShag] == 2)//Если это месяц, то...
					m_strFailImyaFormat += "MM";
				else{
					if(ntMassFailImya[ntShag] == 3)//Если это день, то...
						m_strFailImyaFormat += "dd";
					else{
						if(ntMassFailImya[ntShag] == 4)//Если это чёрточка, то...
							m_strFailImyaFormat += "-";
						else{
							if(ntMassFailImya[ntShag] == 5)//Если это пробел, то...
								m_strFailImyaFormat += " ";
						}
					}
				}
			}
		}

		m_ntKolichestvo = 0;//Обнуляем.
		m_blRekursiya = true;//Запускаем рекурсию.
		m_ntSchet = 0;//Обнуляем количество копируемых файлов.
		m_strSortPut = strSortPut;//Делаем путь сортировки глобальным.
		startSchet(QDir(strLibPut));//Подсчёт копируемых файлов.
		if(m_ntSchet){//Если колличество сортируемых файлов !=0, то...
			ui->ppbrSort->setRange(0, m_ntSchet);//Задаю количество шагов от 0 до 100%.
			startSort(QDir(strLibPut));//Запускаем сортировку файлов.
			ui->ppbrSort->setValue(m_ntSchet);//Отобразить на панели прогресса 100%.
		}
	}
	else{//В противном случае...
		ui->pgbxSort->setEnabled(true);//Активируем меню.
		m_pdcmessage->critical(tr("Ошибка:"), tr("Ошибка открытия списка удаляемых файлов. ") +
					tr("Закройте все программы и попробуйте запустить сортировку заново."));//
		return;//Ошибка записи в файл.
	}
	slotSortRabota();//Показать время работы сортировщика.
	m_pdcvremya->stopTimer();//Останавливаем таймер обновления времени.
	m_pflCopy->close();//Закрываем файл на чтение.
	ui->pgbxSort->setEnabled(true);//Активируем меню.
	ui->plcdSortKolichestvo->display(m_ntKolichestvo);//Отобразить количество отсортированных файлов.
	m_pdcmessage->information(sozdat, tr("Сортировка файлов завершена! Количество отсортированных файлов: ")
			+ QString::number(m_ntKolichestvo) + ".");//

	if(m_ntSchet){//Если количество сортуруемых файлов было !=0, то...
		QMessageBox msgBox(this);//Создаем объект Сообщения
		msgBox.setWindowTitle(tr("Удалить:"));//Устанавливаем заголовок окна сообщения.
		msgBox.setIcon(QMessageBox::Question);
		msgBox.setText(tr("Удалить оригиналы файлов?"));//
		msgBox.setInformativeText(tr("Удалить?"));//Добавляем вопрос в сообщение
		//Создаем кнопку Удалить, и добавляем ее в сообщение
		QPushButton* pcmdOk = msgBox.addButton(tr("Удалить"), QMessageBox::ActionRole);
		//Создаем кнопку Отмена и добавляем ее в сообщение
		QPushButton* pcmdChancel = msgBox.addButton(tr("Отмена"), QMessageBox::ActionRole);
		msgBox.setDefaultButton(pcmdChancel);//Делаем по умолчанию выделенную кнопку Ок
		msgBox.exec();//Запускаем Сообщение
		if(msgBox.clickedButton() == pcmdChancel){//Если нажата кнопка Отмена, то...
		}
		if (msgBox.clickedButton() == pcmdOk){//Если нажата кнопка Ок, то...
			if(udalitFaili()){//Если удаление файлов прошло без ошибок, то...
				m_pdcmessage->information(udalit, tr("Удаление файлов прошло успешно!"));//
			}
		}
	}
	//---обязательно-удалить-список-удаляемых-файлов!!!---//
	if(m_pflCopy->exists()){//Если файл в папке есть, то...
		if(!m_pflCopy->remove()){//Удаляем файл. Если он не удалился, то...
			m_pdcmessage->critical(tr("Ошибка:"), tr("При удалении файла: ") + strProgram 
					+ tr(" произошла ошибка. Закройте все программы и попробуйте заново."));
			return;//Выходим из слота.
		}
	}
}

void DCSort::slotSortProvodnik(){//Слот открытия папки отсортированных файлов.
/////////////////////////////////////////////////////////
//---С Л О Т   О Т К Р Ы Т И Я   П Р О В О Д Н И К А---//
/////////////////////////////////////////////////////////
	QString strSortPut = ui->pldtSortPut->text();//Путь к месту сортировки.
	QDir drSortPut(strSortPut);//Папка места сортировки файлов
	if(!drSortPut.exists()){//Если данная папка не существует, то...
		m_pdcmessage->critical(ui->plblSortPut, tr("Данной папки не существует: ") + ui->plblSortPut->text()
				+ tr(" , укажите существующую папку!"));
	}
	else{
		if(strSortPut.isEmpty()){//Если путь места сортировки пуст, то...
			m_pdcmessage->critical(ui->plblSortPut, tr("Пустое поле: ") + ui->plblSortPut->text()
					+ tr(", заполните его!"));
		}
		else{//В противном случае...
			QFileDialog::getOpenFileName(this,tr("Отсортированные файлы:"),ui->pldtSortPut->text(),
					tr("Типы файлов: (")+polMasku()+")");//Открываем проводник, в котором файлы
		}
	}
}

void DCSort::slotSortOtmena(){//Слот отмены процесса сортировки.
/////////////////////////////////////////////////////
//---С Л О Т   О Т М Е Н Ы   С О Р Т И Р О В К И---//
/////////////////////////////////////////////////////
	m_blRekursiya = false;
}

void DCSort::slotSortVihod(){//Слот выхода из программы.
///////////////////////////////////////////
//---С Л О Т   К Н О П К И   В Ы Х О Д---//
///////////////////////////////////////////
	this->close();//Закрыть программу.
}

void DCSort::slotSortRabota(){//Слот отображающий время работы Сортировки.
/////////////////////////////////////////////
//---С Л О Т   Т А Й М Е Р   Р А Б О Т Ы---//
/////////////////////////////////////////////
	QTime tmSeichas = QTime::currentTime();//Время в данный момент.
	QTime tmRabota(0,0,0);//Расчётное время работы программы.
	tmRabota = tmRabota.addSecs(abs((int)m_ptmStart->secsTo(tmSeichas)));//Расчит. время от старта до сечас.
	ui->plblSortRabotaTimer->setText(tmRabota.toString("hh:mm:ss"));//Отображаем расчётное время сортировки.
}
