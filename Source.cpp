#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <time.h>

#include <locale.h>

#include <iostream>

#include <vector>
#include <windows.h>

using namespace std;

struct flight

{

	time_t time_departure; // время вылета (в сек.)

	long int time_travel, time_arrival; // время в пути и время окончания рейса (в сек.)

	int number_flight; //номер рейса

	int number_seats; //кол-во посадочных мест в самолете

	int number_available_seats; //кол-во свободных (непроданых и незабронированых) мест

	int number_tickets_sold; //кол-во проданых билетов

	int number_tickets_booked; //кол-во забронированных билетов

	int status;// 0 - отменен 1 - бронирование 2 - только покупка 3 - в полете 4 - завершен

    string initial_point; // начальный пункт

	string terminal_point; // конечный пункт

	struct passengers
	{

		string name;

		string surname;

		bool status;

		passengers(string name, string surname, int status) :name(name), surname(surname), status(status)

		{

		};

	};

	vector <passengers> list_passengers;//список пассажиров

	flight(int nuber_flight, string initial_point, string terminal_point, time_t time_departure, long int time_travel, int number_seats)

	{

		time_t time_now = time(NULL);

		this->number_flight = nuber_flight;

		this->number_seats = number_seats;

		this->initial_point = initial_point;

		this->terminal_point = terminal_point;

		this->time_departure = time_departure;

		this->time_travel = time_travel;

		time_arrival = (int)time_departure + time_travel;

		number_tickets_sold = 0;

		number_tickets_booked = 0;

		number_available_seats = number_seats;

		status = 1;

		if (time_now > time_departure - 2592000) status = 2;

		if (time_now > time_departure) status = 3;

		if (time_now > time_arrival) status = 4;

	}

	bool Booking(string name, string surname) //бронирование билета на рейс

	{

		if (number_available_seats == 0) return 0;

		passengers Passengers(name, surname, 1);

		list_passengers.push_back(Passengers);

		number_available_seats--;

		number_tickets_booked++;

		return 1;

	}

	bool Buying(string name, string surname) //покупка билета на рейс

	{

		if (number_available_seats == 0) return 0;

		passengers Passengers(name, surname, 0);

		list_passengers.push_back(Passengers);

		number_available_seats--;

		number_tickets_sold++;

		return 1;

	}

	void PrintListFligts()//вывод списка пассажиров рейса

	{

		if (number_seats - number_available_seats == 0) printf("Пасажиры отсутствуют\n");

		else

		{

			printf("\nСписок пасажиров\n");

			for (int i = 0; i < number_seats - number_available_seats; i++)

				printf("%d. %s %s\n", i + 1, list_passengers[i].name.c_str(), list_passengers[i].surname.c_str());

		}

	}

	void PrintInfo(bool f) //вывод информации о рейсе

	{

		struct tm* timeinfo;

		timeinfo = gmtime(&time_departure);

		printf("Рейс №%d\n", number_flight);

		printf("Маршрут следования %s - %s\n", initial_point.c_str(), terminal_point.c_str());

		printf("Дата вылета: %02d.%02d.%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

		printf("Время вылета %02d:%02d\n", timeinfo->tm_hour + 7, timeinfo->tm_min);

		printf("Время в пути %02ld:%02ld\n", time_travel / 3600, (time_travel - (time_travel / 3600) * 3600) / 60);

		printf("Всего мест: %d\n", number_seats);

		printf("Свободно мест: %d\n", number_available_seats);

		printf("Забронированно мест: %d\n", number_tickets_booked);

		printf("Куплено мест: %d\n", number_tickets_sold);

		if (f) PrintListFligts();

	}

};

bool checkData(int day, int month, int year) //проверка кореектности вводимой даты

{

	if (day < 1 || day>31)

		return 0;

	if (month < 1 || month>12)

		return 0;

	if (day == 31 && !(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12))

		return 0;

	if (day == 30 && !(month == 4 || month == 6 || month == 9 || month == 11))

		return 0;

	if (day == 29 && month == 2 && (year % 4 != 0 || (year % 100 == 0 && year % 4 != 0)))

		return 0;

	if (year < 2000 || year>2030)

		return 0;

	return 1;

}

bool checkTime(int hour, int minets) //проверка кореектности вводимого времени

{

	if (minets < 0 || minets>59)

		return 0;

	if (hour < 0 || hour>23)

		return 0;

	return 1;

}

void PrintFlights(vector <flight>& list_flight, int n, int status) //вывод всех рейсов с указаным статусом

{

	printf("\n-------------------------------------------------\n");

	switch (status)
	{

	case 0:

		printf("Отмененные рейсы:\n");

		break;

	case 1:

		printf("Рейсы доступные для бронирования и покупки:\n");

		break;

	case 2:

		printf("Рейсы доступные для покупки:\n");

		break;

	case 3:

		printf("Рейсы находящиеся в пути:\n");

		break;

	case 4:

		printf("Завершенные рейсы:\n");

		break;

	}

	for (int i = 0; i <= n; i++)

		if (list_flight[i].status == status)

		{

			printf("\n-------------------------------------------------\n");

			list_flight[i].PrintInfo(0);

			printf("\n-------------------------------------------------\n");

		}

	printf("\n-------------------------------------------------\n");

}

void PrintAllFlights(vector <flight>& list_flight, int& n) //вывод информации о всех рейсах

{

	for (int i = 0; i <= 4; i++)

		PrintFlights(list_flight, n, i);

}

void PrintListPassengers(vector <flight>& list_flight, int& n) //вывод списка пасажиров

{

	int a;

	do

	{

		printf("Введите номер рейса, список пасcажиров которого вывести (для выхода введите 0)\n");

		scanf("%d", &a);

		if (a == 0)

			return;

		if (a > n || a <= 0)

			printf("Рейс с таким номером отсутствует\n");

	}

	while (a > n || a <= 0);

	list_flight[a].PrintListFligts();

}

void CreateFlight(vector <flight>& list_flight, int& n) //ввод данных для создание рейса

{

	n++;

	time_t timer1 = time(NULL);

	struct tm* timeinfo = localtime(&timer1);

	int number_seats;

	int minutes, hour, day, month, year;

	int minutes2, hour2;

	string initial_point, terminal_point;

	printf("Создание %d рейса \n", n);

	printf("Введите начальный пункт \n");

	cin >> initial_point;

	printf("Введите конечный пункт \n");

	cin >> terminal_point;

	while (1)

	{

		printf("Введите дату вылета в формате дд.мм.гггг\n");

		if (scanf("%d.%d.%d", &day, &month, &year) == 3 && getchar() == '\n' && checkData(day, month, year))
			break;

		printf("Неверный формат даты\n");

	}

	while (1)

	{

		printf("Введите время вылета в формате чч:мм\n");

		if (scanf("%d:%d", &hour, &minutes) == 2 && getchar() == '\n' && checkTime(hour, minutes))

			break;

		printf("Неверный формат времени\n");

	}
	while (1)

	{

		printf("Введите время в пути в формате чч:мм\n");

		if (scanf("%d:%d", &hour2, &minutes2) == 2 && getchar() == '\n' && checkTime(hour2, minutes2))

			break;

		printf("Неверный формат времени\n");

	}

	printf("Введите кол-во мест \n");

	scanf("%d", &number_seats);

	timeinfo->tm_year = year - 1900;

	timeinfo->tm_mon = month - 1;

	timeinfo->tm_mday = day;

	timeinfo->tm_hour = hour;

	timeinfo->tm_min = minutes;

	timeinfo->tm_sec = 0;

	timer1 = mktime(timeinfo);

	flight Flight(n, initial_point, terminal_point, timer1, hour2 * 3600 + minutes2 * 60, number_seats);

	list_flight.push_back(Flight);

}

void CancelFlight(vector <flight>& list_flight, int& n) //отмена рейса

{

	int a;

	do

	{

		printf("Введите номер отменяемого рейса (для выхода введите 0)\n");

		scanf("%d", &a);

		if (a == 0) return;

		if (list_flight[a].status == 0)

			printf("%d Рейс уже отменен\n", a);

		if (list_flight[a].status == 3)

			printf("%d Рейс в пути\n", a);

		if (list_flight[a].status == 4)

			printf("%d Рейс уже завершен\n", a);

		if (a > n || a <= 0)

			printf("Рейс с таким номером отсутствует\n");

	}

	while (a > n || a <= 0);

	if (list_flight[a].status >= 1 && list_flight[a].status <= 2)

		list_flight[a].status = 0;

	printf("%d рейс успешно отменен", a);

}

void BuyingTicket(vector <flight>& list_flight, int& n) //ввод данных для совершения покупки билета на рейс

{

	int a;

	string name, surname;

	PrintFlights(list_flight, n, 1);

	PrintFlights(list_flight, n, 2);

	printf("Введите номер рейса, на который вы хотите преобрести билет (для выхода введите 0)\n");

	do

	{

		scanf("%d", &a);

		if (a == 0) return;

		if (a > n || a < 0)

			printf("Рейс с таким номером отсутствует\n");

		if (list_flight[a].status > 2 || list_flight[a].status < 1)

			printf("Покупка билетов на данный рейс недоступна\n");

	}

	while (a > n || a < 0 || list_flight[a].status>2 || list_flight[a].status < 1);

	printf("Введите имя \n");

	getchar();

	cin >> name;

	printf("Введите фамилию \n");

	cin >> surname;

	if (list_flight[a].Buying(name, surname))

		printf("Успешно\n");

	else

		printf("Ошибка. Недостаточно свободных мест\n");

}

void BookingTicket(vector <flight>& list_flight, int& n) //ввод данных для совершения бронирования билета на рейс

{

	int a;

	string name, surname;

	PrintFlights(list_flight, n, 1);

	printf("Введите номер рейса, на который вы хотите забронировать билет (для выхода введите 0)\n");

	do

	{

		scanf("%d", &a);

		if (a == 0) return;

		if (a > n && a < 0)

			printf("Рейс с таким номером отсутствует\n");

		if (list_flight[a].status != 1)

			printf("Бронирование билетов на данный рейс недоступна\n");

	}

	while (a > n || a < 0 || list_flight[a].status != 1);

	getchar();

	printf("Введите имя \n");

	getchar();

	cin >> name;

	printf("Введите фамилию \n");

	cin >> surname;

	if (list_flight[a].Booking(name, surname))

		printf("Успешно\n");

	else printf("Ошибка. Недостаточно свободных мест\n");

}
void SearchFlight(vector <flight>& list_flight, int& n) //поиска рейса по начальному и конечному пункту

{

	int a;

	printf("1 - Искать по начальному пункту\n2 - Искать по конечному пункту\n3 - Искать по начальному и конечному пункту\n0 - Выйти в меню\n");

	while (1)

	{

		scanf("%d", &a);

		if (a == 0) return;

		if (a <= 3 && a >= 1) break;

		printf("Неверный формат ввода. Введите число от 0 до 3\n");

	}

	string initial_point, terminal_point;

	if (a != 2)

	{

		printf("Введите название пункта вылета \n");

		getchar();

		cin >> initial_point;

	}

	if (a != 1)

	{

		printf("Введите название пункта прилета \n");

		cin >> terminal_point;

	}

	printf("Вам подойдут следующие рейсы ");

	switch (a)

	{

	case 1:

		for (int i = 0; i <= n; i++)

			if (list_flight[i].initial_point == initial_point && list_flight[i].status <= 2)

			{

				printf("\n-------------------------------------------------\n");

				list_flight[i].PrintInfo(0);

				printf("\n-------------------------------------------------\n");

			}

		break;

	case 2:

		for (int i = 0; i <= n; i++)

			if (list_flight[i].terminal_point == terminal_point && list_flight[i].status <= 2)

			{

				printf("\n-------------------------------------------------\n");

				list_flight[i].PrintInfo(0);

				printf("\n-------------------------------------------------\n");

			}

		break;

	case 3:

		for (int i = 0; i <= n; i++)

			if (list_flight[i].initial_point == initial_point && list_flight[i].terminal_point == terminal_point && list_flight[i].status <= 2)

			{

				printf("\n-------------------------------------------------\n");

				list_flight[i].PrintInfo(0);

				printf("\n-------------------------------------------------\n");

			}

		break;

	}

	printf("\n");

}

void CheckStatus(vector <flight>& list_flight, int& n) //обновление статусов рейсов

{

	time_t time_now = time(NULL);

	for (int i = 1; i <= n; i++)

	{

		if (list_flight[i].status <= 3 && list_flight[i].status != 0 && time_now > list_flight[i].time_arrival)

		{

			list_flight[i].status = 4;

			printf("Статус %d рейса переведен на *Окончен*\n", i);

		}

		if (list_flight[i].status <= 2 && list_flight[i].status != 0 && time_now > list_flight[i].time_departure)

		{

			list_flight[i].status = 3;

			printf("Статус %d рейса переведен на *Начат*\n", i);

		}

		if (list_flight[i].status == 1 && time_now > list_flight[i].time_departure - 2592000)

		{

			list_flight[i].status = 2;

			printf("Статус %d рейса переведен на *Только покупка*\n", i);

		}

	}

	printf("----------------Успешно обновленно----------------\n");

}

void RandomGeneration(vector <flight>& list_flight, int& n) //генерация рейсов с списком пасажиров

{

	int k;

	string city[] =

	{ "Сидней" ,"Мельбурн" ,"Брисбен" ,"Перт" ,"Аделаида" ,"Окленд" ,"Баку" ,"Ереван" ,"Дакка" ,"Читтагонг" ,"Нараянгандж" ,"Кхулна" ,"Газипур" ,"Хошимин" ,"Ханой" ,"Тбилиси" ,"Бомбей" ,"Дели" ,"Бангалор" ,"Ахмадабад" ,"Калькутта" ,"Сурат" ,"Джайпур" ,"Ахмадабад" ,"Пуна" ,"Лакхнау" ,"Канпур" ,"Тхане" ,"Нагпур" ,"Патна" ,"Агра" ,"Бхопал" ,"Газиабад" ,"Лухдхияна" ,"Вишакхапатнам" ,"Индуар" ,"Вадодара" ,"Миратх" ,"Варанаси" ,"Фаридабад" ,"Раджкот" ,"Аурангабад" ,"Аллахабад" ,"Пимпри-Чинчвад" ,"Асансол" ,"Васаи-Вирар" ,"Кальян" ,"Сринагар" ,"Туриппур" ,"Нишик" ,"Амритсар" ,"Ранчи" ,"Нави-Мумбаи" ,"Барали" ,"Майсур" ,"Ховрах" ,"Бхилаи" ,"Джабалпур" ,"Гвалиор" ,"Коимбатур" ,"Виджаявада" ,"Гувахати" ,"Джодхпур" ,"Чандигарх" ,"Алигарх" ,"Мадурай" ,"Морадабад" ,"Кота" ,"Джакарта" ,"Сурабая" ,"Бандунг" ,"Бекаси" ,"Медан" ,"Серанг" ,"Семаранг" ,"Тангеранг" ,"Депок" ,"Макасар" ,"Палембанг" ,"Маланг" ,"Пеканбару" ,"Паданг" ,"Амман" ,"Багдад" ,"Басра" ,"Эрбиль" ,"Мосул" ,"Сулеймания" ,"Тегеран" ,"Мешхед" ,"Исфахан" ,"Кередж" ,"Тебриз" ,"Шираз" ,"Сана" ,"Алма-Ата" ,"Шанхай" ,"Пекин" ,"Шэньчжэнь" ,"Тяньцзинь" ,"Ухань" ,"Чэнду" ,"Ханчжоу" ,"Чунцин" ,"Дунгуань" ,"Нанкин" ,"Шэньян" ,"Сиань" ,"Сучжоу" ,"Чжэнчжоу" ,"Харбин" ,"Циндао" ,"Далянь" ,"Куньмин" ,"Цзинань" ,"Нинбо" ,"Наньтун" ,"Тайюань" ,"Хэфэй" ,"Чанша" ,"Уси" ,"Ханчжоу" ,"Фошань" ,"Чанчунь" ,"Шизцячжуан" ,"Вэньчжоу" ,"Гуйян" ,"Урумчи" ,"Хух-Хото" ,"Ланьчжоу" ,"Наньчан" ,"Наньнин" ,"Чжуншань" ,"Линьи" ,"Шатьтоу" ,"Гуанчжоу" ,"Таньшань" ,"Хуайань" ,"Баотоу" ,"Цзаочжуан" ,"Цзинин" ,"Цзюлун" ,"Ханьдань" ,"Лоян" ,"Куньшань" ,"Хойчжоу" ,"Датун" ,"Иу" ,"Гирин" ,"Яньчэн" ,"Нучжуань" ,"Тайчжоу" ,"Вэйфан" ,"Лючжоу" ,"Яньжноу" ,"Цзыбо" ,"Аншань" ,"Яньтай" ,"Путянь" ,"Уху" ,"Жугао" ,"Цюаньчжоу" ,"Сямынь" ,"Лоян" ,"Линьчжоу" ,"Аньян" ,"Тайчжоу" ,"Пинду" ,"Синин" ,"Чаншу" ,"Цзяньмэнь" ,"Цзямусы" ,"Фушунь" ,"Сюйчжоу" ,"Боадин" ,"Юйчжоу" ,"Гонконг" ,"Сюйчан" ,"Чжанцзяган" ,"Цицикар" ,"Фуцинь" ,"Юйшу" ,"Чифэн" ,"Вэйхай" ,"Дунъин" ,"Цанчжоу" ,"Цзясин" ,"Чжэньцзян" ,"Синъян" ,"Дацин" ,"Цзимо" ,"Дуньтай" ,"Динчжоу" ,"Чжоучжэн" ,"Шаосин" ,"Чжанцзякоу" ,"Хуайнань" ,"Жуйан" ,"Хэйчэн" ,"Ляньюньган" ,"Цидун" ,"Хуайбэй" ,"Тэнчжоу" ,"Цзиньчжоу" ,"Чжучэн" ,"Синьсян" ,"Чжучжоу" ,"Фучжоу" ,"Пунин" ,"Чжанцзян" ,"Циньхуаньдао" ,"Ичан" ,"Цзиньцзян" ,"Цыси" ,"Хучжоу" ,"Чжанцю" ,"Сантань" ,"Тайбей" ,"Синьбэй" ,"Гаосюн" ,"Тайчжун" ,"Пхеньян" ,"Сеул" ,"Пусан" ,"Инчхон" ,"Тэгу" ,"Кванджу" ,"Тэджон" ,"Сувон" ,"Ульсан" ,"Чханвон" ,"Соннам" ,"Бейрут" ,"Каула-Лумпур" ,"Улан-Батор" ,"Ягон" ,"Мандалай" ,"Нейпьидо" ,"Катманду" ,"Карачи" ,"Лахор" ,"Фейсалабад" ,"Хайдарабад" ,"Мултан" ,"Равалпинди" ,"Пешавар" ,"Гуджранвала" ,"Новосибирск" ,"Челябинск" ,"Омск" ,"Красноярск" ,"Эр-Рияд" ,"Джидда" ,"Мекка" ,"Даммам" ,"Сингапур" ,"Халеб" ,"Дамаск" ,"Бангкок" ,"Стамбул" ,"Анкара" ,"Измир" ,"Бурса" ,"Адана" ,"Газиантеп" ,"Анталья" ,"Ташкент" ,"Манила" ,"Давао" ,"Калукан" ,"Токио" ,"Йокогама" ,"Осака" ,"Нагоя" ,"Саппоро" ,"Кобе" ,"Фукуока" ,"Киото" ,"Кавасаки" ,"Сайтама" ,"Хиросима" ,"Седай" ,"Алжир" ,"Луанда" ,"Аккра" ,"Кумаси" ,"Конкари" ,"Киншаса" ,"Лубумбаши" ,"Мбужи-Майи" ,"Кананга" ,"Каир" ,"Александрия" ,"Гиза" ,"Хараре" ,"Дуала" ,"Яунде" ,"Найроби" ,"Абиджан" ,"Триполи" ,"Антананариву" ,"Бамако" ,"Касабланка" ,"Рабат" ,"Фес" ,"Мапуту" ,"Лагос" ,"Ибадан" ,"Бенин-Сити" ,"Кано" ,"Порт-Харкорт" ,"Кадуна" ,"Аба" ,"Абуджа" ,"Майдугури" ,"Илорин" ,"Варри" ,"Браззавиль" ,"Дакар" ,"Могадишо" ,"Хартум" ,"Омдурман" ,"Дар-Эс-Салам" ,"Тунис" ,"Кампала" ,"Адис-Абеба" ,"Йоханнесбург" ,"Кейптаун" ,"Дурбан" ,"Претория" ,"Вена" ,"Минск" ,"София" ,"Лондон" ,"Бирмингем" ,"Будапешт" ,"Берлин" ,"Гамбург" ,"Мюнхен" ,"Кёльн" ,"Мадрид" ,"Барселона" ,"Рим" ,"Милан" ,"Варшава" ,"Москва" ,"Санкт-Петербург" ,"Нижний" ,"Казань" ,"Самара" ,"Ростов-на-Дону" ,"Уфа" ,"Волгоград" ,"Пермь" ,"Воронеж" ,"Бухарест" ,"Белград" ,"Киев" ,"Харьков" ,"Одесса" ,"Днепропетровск" ,"Париж" ,"Прага" ,"Republic" ,"Гватемала" ,"Тегусигальпа" ,"Санто-Доминго" ,"Торонто" ,"Калгари" ,"Гавана" ,"Мехико" ,"Гвадалахара" ,"Пуэбла-дэ-Саргоса" ,"Леон" ,"Сьюдад-Хуарес" ,"Тихуана" ,"Сапопан" ,"Несауалькойотль" ,"Монтеррей" ,"Манагуа" ,"Панама" ,"Нью-Йорк" ,"Лос-Анжелес" ,"Чикаго" ,"Хьюстон" ,"Финикс" ,"Филадельфия" ,"Сан-Антонио" ,"Даллас" ,"Буэнос-Айерес" ,"Кордова" ,"Санта-Крус-де-ла-Сьерра" ,"Сан-Паулу" ,"Рио-де-Жанейеро" ,"Салвадор" ,"Бразилиа" ,"Форталеза" ,"Белу-Оризонти" ,"Куритиба" ,"Манаус" ,"Ресифи" ,"Порту-Алгери" ,"Белен" ,"Гояния" ,"Гаурульюс" ,"Сан-Луис" ,"Кампинас" ,"Каракас" ,"Маракайбо" ,"Баркисимето" ,"Богота" ,"Медельин" ,"Кали" ,"Барранкилья" ,"Лима" ,"Монтевидео" ,"Сантьяго" ,"Гауякиль" ,"Кито" };

	string name[] =

	{ "Абрам" ,"Аваз" ,"Август" ,"Авдей" ,"Автандил" ,"Адам" ,"Адис" ,"Адольф" ,"Адриан" ,"Азарий" ,"Аким" ,"Алан " ,"Александр" ,"Алексей" ,"Альберт" ,"Альфред" ,"Амадей" ,"Амадеус" ,"Амаяк" ,"Анатолий" ,"Ангел" ,"Андоим" ,"Андрей" ,"Аникита" ,"Антон" ,"Ануфрий " ,"Арам" ,"Арий" ,"Аристарх" ,"Аркадий" ,"Арно" ,"Арнольд" ,"Арон" ,"Арсен" ,"Артем" ,"Артемий" ,"Артур" ,"Архип" ,"Аскольд" ,"Афанасий" ,"Ахмет" ,"Ашот" ,"Бежен " ,"Бенедикт" ,"Берек" ,"Бернар" ,"Богдан" ,"Боголюб" ,"Болеслав" ,"Бонифаций" ,"Борис" ,"Борислав" ,"Боян" ,"Бронислав" ,"Бруно" ,"Вадим" ,"Валентин" ,"Валерий" ,"Вальтер" ,"Василий" ,"Велизар" ,"Венедикт" ,"Вениамин" ,"Виктор" ,"Вилен" ,"Вилли" ,"Вильгельм" ,"Виссарион" ,"Виталий" ,"Витаутас" ,"Витольд" ,"Владимир" ,"Владислав" ,"Владлен" ,"Володар" ,"Вольдемар" ,"Всеволод" ,"Вячеслав" ,"Гавриил" ,"Гарри" ,"Гастон" ,"Геннадий" ,"Генрих" ,"Георгий" ,"Геральд" ,"Герасим" ,"Герман" ,"Глеб" ,"Гордей" ,"Гордон" ,"Градимир" ,"Григорий" ,"Гурий" ,"Давыд " ,"Даниил" ,"Демид " ,"Демьян " ,"Денис" ,"Джордан" ,"Дмитрий" ,"Дональд" ,"Донат" ,"Донатос" ,"Дорофей" ,"Евгений" ,"Евграф" ,"Евдоким" ,"Евстафий" ,"Егор" ,"Елизар" ,"Елисей" ,"Емельян" ,"Ермолай" ,"Ерофей" ,"Ефим" ,"Ефимий" ,"Ефрем" ,"Жан" ,"Ждан" ,"Жорж" ,"Захар" ,"Захария" ,"Зигмунд" ,"Зиновий" ,"Ибрагим" ,"Иван" ,"Игнат" ,"Игорь" ,"Измаил" ,"Израиль" ,"Илиан" ,"Илларион" ,"Илья"

	,"Иннокентий" ,"Ион" ,"Ионос" ,"Иосиф" ,"Ираклий" ,"Иржи" ,"Исай" ,"Казимир" ,"Карен" ,"Карл" ,"Ким" ,"Кирилл" ,"Клавдий" ,"Клемент" ,"Клим " ,"Клод" ,"Кондрат" ,"Конкордий" ,"Константин" ,"Кузьма" ,"Лазарь" ,"Лев" ,"Леван" ,"Леонард" ,"Леонид" ,"Леонтий" ,"Леопольд" ,"Лука" ,"Любомир" ,"Людвиг" ,"Люсьен" ,"Мадлен" ,"Май" ,"Макар" ,"Максим" ,"Максимилиан" ,"Мануил" ,"Марат" ,"Мариан" ,"Марк" ,"Мартин " ,"Матвей" ,"Мераб" ,"Мечеслав " ,"Милан" ,"Мирон" ,"Мирослав" ,"Михаил" ,"Мичлов" ,"Модест" ,"Моисей" ,"Мурат" ,"Муслим" ,"Назар" ,"Назарий" ,"Натан" ,"Наум" ,"Никита" ,"Никифор" ,"Николай" ,"Никон" ,"Нисон" ,"Нифонт" ,"Олан" ,"Олег" ,"Олесь" ,"Онисим" ,"Орест" ,"Осип" ,"Оскар" ,"Павел" ,"Парамон" ,"Петр" ,"Платон" ,"Порфирий" ,"Прохор" ,"Равиль" ,"Радий " ,"Радомир" ,"Раис" ,"Раймонд" ,"Ратмир" ,"Рафаил" ,"Рафик" ,"Рашид" ,"Рем" ,"Ренольд" ,"Ринат " ,"Рифат" ,"Ричард" ,"Роберт" ,"Родион" ,"Ролан" ,"Роман" ,"Ростислав" ,"Рубен" ,"Рудольф" ,"Руслан" ,"Рустам" ,"Савва" ,"Савел " ,"Самсон" ,"Святослав" ,"Севастьян" ,"Северин" ,"Семен" ,"Серафим" ,"Сергей" ,"Сократ" ,"Соломон" ,"Спартак" ,"Стакрат" ,"Станислав" ,"Степан" ,"Стивен" ,"Стоян" ,"Таис" ,"Талик" ,"Тамаз" ,"Тарас" ,"Тельман" ,"Теодор" ,"Терентий" ,"Тибор" ,"Тигран" ,"Тигрий" ,"Тимофей" ,"Тимур" ,"Тит" ,"Тихон" ,"Трифон" ,"Трофим" ,"Ульманас" ,"Устин" ,"Фаддей" ,"Федор" ,"Феликс" ,"Феодосий" ,"Фидель" ,"Филимон" ,"Филипп" ,"Флорентий" ,"Фома" ,"Франц" ,"Фридрих" ,"Харитон" ,"Христиан" ,"Христос" ,"Христофор" ,"Эдвард" ,"Эдуард" ,"Эльдар" ,"Эмиль" ,"Эммануил" ,"Эраст" ,"Эрик" ,"Эрнест" ,"Юлиан" ,"Юрий" ,"Юхим" ,"Яким" ,"Яков" ,"Ян" ,"Яромир" ,"Ярослав" ,"Ясон" ,"Августа" ,"Аврора" ,"Агата" ,"Агнесса" ,"Агния" ,"Ада" ,"Адель" ,"Аза" ,"Азиза" ,"Аида" ,"Алана" ,"Алевтина" ,"Александра" ,"Алико" ,"Алина" ,"Алиса" ,"Алла" ,"Альберта" ,"Альбина" ,"Альжбета" ,"Амелия " ,"Амина" ,"Анастасия" ,"Ангелина" ,"Анжела" ,"Анисья" ,"Анита" ,"Анна" ,"Антонина" ,"Анфиса" ,"Анэля" ,"Ариадна" ,"Арина" ,"Архелия" ,"Астра" ,"Аурелия" ,"Беатриса" ,"Белла" ,"Береслава" ,"Берта" ,"Биргит" ,"Богдана" ,"Божена" ,"Борислава" ,"Бронислава" ,"Валентина" ,"Валерия" ,"Ванда" ,"Варвара" ,"Василиса" ,"Венера" ,"Вера" ,"Вероника" ,"Веселина" ,"Веста" ,"Вета" ,"Вида" ,"Виктория" ,"Вилора" ,"Виолетта" ,"Виргиния" ,"Виталина" ,"Владислава" ,"Галина" ,"Гаянэ" ,"Гелена" ,"Гелла" ,"Генриетта" ,"Георгина" ,"Гера" ,"Гертруда" ,"Глафира" ,"Глория" ,"Гражина" ,"Грета" ,"Гюзель" ,"Дайна" ,"Дана" ,"Даниэла" ,"Данута" ,"Дарина" ,"Дарья" ,"Дебора" ,"Джемма" ,"Джулия" ,"Джульетта" ,"Диана" ,"Дина" ,"Динара" ,"Диодора" ,"Дионисия" ,"Доля" ,"Доминика" ,"Ева" ,"Евгения" ,"Евдокия" ,"Екатерина" ,"Елена" ,"Елизавета" ,"Жанна" ,"Зара" ,"Земфира" ,"Зинаида" ,"Злата" ,"Зоя" ,"Иветта" ,"Ивона" ,"Изабелла" ,"Изольда" ,"Илзе" ,"Инара" ,"Инга" ,"Инесса" ,"Инна" ,"Иоанна" ,"Иоланта" ,"Ираида" ,"Ирина" ,"Ольга" ,"Сабина" ,"Санта" ,"Сарра" ,"Светлана" ,"Северина" ,"Серафима" ,"Сильва" ,"Сима" ,"Симона" ,"Снежана" ,"Софья" ,"Станислава" ,"Стелла" ,"Стефания" ,"Сусанна" ,"Таира" ,"Таисия" ,"Тала" ,"Тамара" ,"Татьяна" ,"Тереза" ,"Томила" ,"Ульяна" ,"Устина" ,"Фаиза" ,"Фаина" ,"Фаня" ,"Фая" ,"Фелиция" ,"Флора" ,"Франсуаза" ,"Фрида" ,"Хильда" ,"Христина" ,"Христя" ,"Цветана" ,"Чеслава" ,"Эдда" ,"Эдита" ,"Элеонора" ,"Элина" ,"Элла" ,"Эллада" ,"Элоиза" ,"Эльвира" ,"Эльга" ,"Эльза" ,"Эльмира" ,"Эмилия" ,"Эмма" ,"Эрика" ,"Эсмеральда" ,"Юзефа" ,"Юлия" ,"Юна" ,"Юнона" ,"Юстина" ,"Ядвига" ,"Яна" ,"Янита" ,"Янка" ,"Ярослава" };

	string surname[] =

	{ "Иванов" ,"Смирнов" ,"Кузнецов" ,"Попов" ,"Васильев" ,"Петров" ,"Соколов" ,"Михайлов" ,"Новиков" ,"Федоров" ,"Морозов" ,"Волков" ,"Алексеев" ,"Лебедев" ,"Семенов" ,"Егоров" ,"Павлов" ,"Козлов" ,"Степанов" ,"Николаев" ,"Орлов" ,"Андреев" ,"Макаров" ,"Никитин" ,"Захаров" ,"Зайцев" ,"Соловьев" ,"Борисов" ,"Яковлев" ,"Григорьев" ,"Романов" ,"Воробьев" ,"Сергеев" ,"Кузьмин" ,"Фролов" ,"Александров" ,"Дмитриев" ,"Королев" ,"Гусев" ,"Киселев" ,"Ильин" ,"Максимов" ,"Поляков" ,"Сорокин" ,"Виноградов" ,"Ковалев" ,"Белов" ,"Медведев" ,"Антонов" ,"Тарасов" ,"Жуков" ,"Баранов" ,"Филиппов" ,"Комаров" ,"Давыдов" ,"Беляев" ,"Герасимов" ,"Богданов" ,"Осипов" ,"Сидоров" ,"Матвеев" ,"Титов" ,"Марков" ,"Миронов" ,"Крылов" ,"Куликов" ,"Карпов" ,"Власов" ,"Мельников" ,"Денисов" ,"Гаврилов" ,"Тихонов" ,"Казаков" ,"Афанасьев" ,"Данилов" ,"Савельев" ,"Тимофеев" ,"Фомин" ,"Чернов" ,"Абрамов" ,"Мартынов" ,"Ефимов" ,"Федотов" ,"Щербаков" ,"Назаров" ,"Калинин" ,"Исаев" ,"Чернышев" ,"Быков" ,"Маслов" ,"Родионов" ,"Коновалов" ,"Лазарев" ,"Воронин" ,"Климов" ,"Филатов" ,"Пономарев" ,"Голубев" ,"Кудрявцев" ,"Прохоров" ,"Наумов" ,"Потапов" ,"Журавлев" ,"Овчинников" ,"Трофимов" ,"Леонов" ,"Соболев" ,"Ермаков" ,"Колесников" ,"Гончаров" ,"Емельянов" ,"Никифоров" ,"Грачев" ,"Котов" ,"Гришин" ,"Ефремов" ,"Архипов" ,"Громов" ,"Кириллов" ,"Малышев" ,"Панов" ,"Моисеев" ,"Румянцев" ,"Акимов" ,"Кондратьев" ,"Бирюков" ,"Горбунов" ,"Анисимов" ,"Еремин" ,"Тихомиров" ,"Галкин" ,"Лукьянов" ,"Михеев" ,"Скворцов" ,"Юдин" ,"Белоусов" ,"Нестеров" ,"Симонов" ,"Прокофьев" ,"Харитонов" ,"Князев" ,"Цветков" ,"Левин" ,"Митрофанов" ,"Воронов" ,"Аксенов" ,"Софронов" ,"Мальцев" ,"Логинов" ,"Горшков" ,"Савин" ,"Краснов" ,"Майоров" ,"Демидов" ,"Елисеев" ,"Рыбаков" ,"Сафонов" ,"Плотников" ,"Демин" ,"Молчанов" ,"Игнатов" ,"Литвинов" ,"Ершов" ,"Ушаков" ,"Дементьев" ,"Рябов" ,"Мухин" ,"Калашников" ,"Леонтьев" ,"Лобанов" ,"Кузин" ,"Корнеев" ,"Евдокимов" ,"Бородин" ,"Платонов" ,"Некрасов" ,"Балашов" ,"Бобров" ,"Жданов" ,"Блинов" ,"Игнатьев" ,"Коротков" ,"Муравьев" ,"Крюков" ,"Беляков" ,"Богомолов" ,"Дроздов" ,"Лавров" ,"Зуев" ,"Петухов" ,"Ларин" ,"Никулин" ,"Серов" ,"Терентьев" ,"Зотов" ,"Устинов" ,"Фокин" ,"Самойлов" ,"Константинов" ,"Сахаров" ,"Шишкин" ,"Самсонов" ,"Черкасов" ,"Чистяков" ,"Носов" ,"Спиридонов" ,"Карасев" ,"Авдеев" ,"Воронцов" ,"Зверев" ,"Владимиров" ,"Селезнев" ,"Нечаев" ,"Кудряшов" ,"Седов" ,"Фирсов" ,"Андрианов" ,"Панин" ,"Головин" ,"Терехов" ,"Ульянов" ,"Шестаков" ,"Агеев" ,"Никонов" ,"Селиванов" ,"Баженов" ,"Гордеев" ,"Кожевников" ,"Пахомов" ,"Зимин" ,"Костин" ,"Широков" ,"Филимонов" ,"Ларионов" ,"Овсянников" ,"Сазонов" ,"Суворов" ,"Нефедов" ,"Корнилов" ,"Любимов" ,"Львов" ,"Горбачев" ,"Копылов" ,"Лукин" ,"Токарев" ,"Кулешов" ,"Шилов" ,"Большаков" ,"Панкратов" ,"Родин" ,"Шаповалов" ,"Покровский" ,"Бочаров" ,"Никольский" ,"Маркин" ,"Горелов" ,"Агафонов" ,"Березин" ,"Ермолаев" ,"Зубков" ,"Куприянов" ,"Трифонов" ,"Масленников" ,"Круглов" ,"Третьяков" ,"Колосов" ,"Рожков" ,"Артамонов" ,"Шмелев" ,"Лаптев" ,"Лапшин" ,"Федосеев" ,"Зиновьев" ,"Зорин" ,"Уткин" ,"Столяров" ,"Зубов" ,"Ткачев" ,"Дорофеев" ,"Антипов" ,"Завьялов" ,"Свиридов" ,"Золотарев" ,"Кулаков" ,"Мещеряков" ,"Макеев" ,"Дьяконов" ,"Гуляев" ,"Петровский" ,"Бондарев" ,"Поздняков" ,"Панфилов" ,"Кочетков" ,"Суханов" ,"Рыжов" ,"Старостин" ,"Калмыков" ,"Колесов" ,"Золотов" ,"Кравцов" ,"Субботин" ,"Шубин" ,"Щукин" ,"Лосев" ,"Винокуров" ,"Лапин" ,"Парфенов" ,"Исаков" ,"Голованов" ,"Коровин" ,"Розанов" ,"Артемов" ,"Козырев" ,"Русаков" ,"Алешин" ,"Крючков" ,"Булгаков" ,"Кошелев" ,"Сычев" ,"Синицын" ,"Черных" ,"Рогов" ,"Кононов" ,"Лаврентьев" ,"Евсеев" ,"Пименов" ,"Пантелеев" ,"Горячев" ,"Аникин" ,"Лопатин" ,"Рудаков" ,"Одинцов" ,"Серебряков" ,"Панков" ,"Дегтярев" ,"Орехов" ,"Царев" ,"Шувалов" ,"Кондрашов" ,"Горюнов" ,"Дубровин" ,"Голиков" ,"Курочкин" ,"Латышев" ,"Севастьянов" ,"Вавилов" ,"Ерофеев" ,"Сальников" ,"Клюев" ,"Носков" ,"Озеров" ,"Кольцов" ,"Комиссаров" ,"Меркулов" ,"Киреев" ,"Хомяков" ,"Булатов" ,"Ананьев" ,"Буров" ,"Шапошников" ,"Дружинин" ,"Островский" ,"Шевелев" ,"Долгов" ,"Суслов" ,"Шевцов" ,"Пастухов" ,"Рубцов" ,"Бычков" ,"Глебов" ,"Ильинский" ,"Успенский" ,"Дьяков" ,"Кочетов" ,"Вишневский" ,"Высоцкий" ,"Глухов" ,"Дубов" ,"Бессонов" ,"Ситников" ,"Астафьев" ,"Мешков" ,"Шаров" ,"Яшин" ,"Козловский" ,"Туманов" ,"Басов" ,"Корчагин" ,"Болдырев" ,"Олейников" ,"Чумаков" ,"Фомичев" ,"Губанов" ,"Дубинин" ,"Шульгин" ,"Касаткин" ,"Пирогов" ,"Семин" ,"Трошин" ,"Горохов" ,"Стариков" ,"Щеглов" ,"Фетисов" ,"Колпаков" ,"Чесноков" ,"Зыков" ,"Верещагин" ,"Минаев" ,"Руднев" ,"Троицкий" ,"Окулов" ,"Ширяев" ,"Малинин" ,"Черепанов" ,"Измайлов" ,"Алехин" ,"Зеленин" ,"Касьянов" ,"Пугачев" ,"Павловский" ,"Чижов" ,"Кондратов" ,"Воронков" ,"Капустин" ,"Сотников" ,"Демьянов" ,"Косарев" ,"Беликов" ,"Сухарев" ,"Белкин" ,"Беспалов" ,"Кулагин" ,"Савицкий" ,"Жаров" ,"Хромов" ,"Еремеев" ,"Карташов" ,"Астахов" ,"Русанов" ,"Сухов" ,"Вешняков" ,"Волошин" ,"Козин" ,"Худяков" ,"Жилин" ,"Малахов" ,"Сизов" ,"Ежов" ,"Толкачев" ,"Анохин" ,"Вдовин" ,"Бабушкин" ,"Усов" ,"Лыков" ,"Горлов" ,"Коршунов" ,"Маркелов" ,"Постников" ,"Черный" ,"Дорохов" ,"Свешников" ,"Гущин" ,"Калугин" ,"Блохин" ,"Сурков" ,"Кочергин" ,"Греков" ,"Казанцев" ,"Швецов" ,"Ермилов" ,"Парамонов" ,"Агапов" ,"Минин" ,"Корнев" ,"Черняев" ,"Гуров" ,"Ермолов" ,"Сомов" ,"Добрынин" ,"Барсуков" ,"Глушков" ,"Чеботарев" ,"Москвин" ,"Уваров" ,"Безруков" ,"Муратов" ,"Раков" ,"Снегирев" ,"Гладков" ,"Злобин" ,"Моргунов" ,"Поликарпов" ,"Рябинин" ,"Судаков" ,"Кукушкин" ,"Калачев" ,"Грибов" ,"Елизаров" };

	printf("Введите количество рейсов\n");

	scanf("%d", &k);

	for (int i = 0; i < k; i++)

	{

		n++;

		int r[15];

		time_t timer1 = time(NULL);

		struct tm* timeinfo;

		timeinfo = localtime(&timer1);

		do

		{

			r[1] = rand() % (sizeof(city) / sizeof(city[0]));

			r[2] = rand() % (sizeof(city) / sizeof(city[0]));

		}

		while (r[1] == r[2]);

		do

		{

			r[3] = rand() % 30 + 1;

			r[4] = rand() % 12;

			r[5] = 2010 + rand() % 20;

		}

		while (!checkData(r[3], r[4], r[5]));

		r[6] = rand() % 24;

		r[7] = rand() % 60;

		r[8] = 1 + rand() % 21;

		r[9] = rand() % 60;

		r[10] = 10 + rand() % 100;

		timeinfo->tm_year = r[5] - 1900;

		timeinfo->tm_mon = r[4] - 1;

		timeinfo->tm_mday = r[3];

		timeinfo->tm_hour = r[6];

		timeinfo->tm_min = r[7];

		timeinfo->tm_sec = 0;

		timer1 = mktime(&*timeinfo);

		flight Flight(n, city[r[1]], city[r[2]], timer1, r[8] * 3600 + r[9] * 60, r[10]);

		list_flight.push_back(Flight);

		do

		{

			r[11] = 10 + rand() % 100;

		}

		while (r[11] > r[10]);

		for (int j = 0; j < r[11]; j++)

		{

			r[12] = rand() % (sizeof(name) / sizeof(name[0]));

			r[13] = rand() % (sizeof(surname) / sizeof(surname[0]));

			r[14] = rand() % 2;

			switch (r[14]) {

			case 0:

				list_flight[n].Booking(name[r[12]], surname[r[13]] + "(a)");

				break;

			case 1:

				list_flight[n].Buying(name[r[12]], surname[r[13]] + "a");

				break;

			}

		}

	}

}

int main()

{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	typedef void (*MENU)(vector <flight>&, int&);

	vector <flight> list_flight;

	srand(time(NULL));

	int i, k;

	int n = -1;

	char c;

	const char* ss[] = { "1-Добавить рейс","2-Забронировать билет", "3-Купить билет","4-Найти подходящий рейс" ,"5-Отменить рейс","6-Обновоить информацию о рейсах","7-Вывод информации о всех рейсах","8-Вывод списка пасажиров","9-Случайная генерация ресов","0-Выход" };

	MENU Action[] = { CreateFlight,BookingTicket, BuyingTicket, SearchFlight,CancelFlight,CheckStatus,PrintAllFlights,PrintListPassengers,RandomGeneration };

	k = sizeof(ss) / sizeof(ss[0]);

	for (;;)

	{

		for (i = 0; i < k; i++) puts(ss[i]);

		c = getchar();

		printf("\n-------------------------------------------------\n");

		if (c > '9' || c <= '0')

			exit(1);

		if (c != '6')

			CheckStatus(list_flight, n);

		Action[(c - '0' - 1)](list_flight, n); // По номеру пункта меню (по индексу) из массива выбирается адрес функции

		printf("\n-------------------------------------------------\n");

		printf("\n----------Нажмите Enter для продолжения-----------\n");

		while (getchar() != '\n');

	}

}
