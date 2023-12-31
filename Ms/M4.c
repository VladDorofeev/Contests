#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { SEC_TO_ALARM = 5 };

char *num;
char *save_num;
char *num_to_print;
int do_strcpy;

char *
dec (char *num) {
    size_t length = strlen(num);
    int need_dec = 1;
    for (size_t i = length - 1;; --i) {
        if (!need_dec) {
            break;
        }
        if (num[i] == '0') {
            num[i] = '9';
        } else {
            need_dec = 0;
            num[i] -= 1;
        }
    }
    char *ptr;
    for (ptr = num; *ptr == '0'; ptr++);
    if (*ptr == 0) {
        ptr--;
    }
    return ptr;
}

void
sig_alrm (int sig) {
    write(1, num, strlen(num_to_print));
    printf("\n");
    alarm(SEC_TO_ALARM);
}

void
sig_int (int sig) {
    unsigned int to_alarm = alarm(0);
    to_alarm += '0';
    write(1, &to_alarm, sizeof(unsigned int));
    printf("\n");
    to_alarm -= '0';
    alarm(to_alarm);
}


int
main (int argc, char **argv) {
    num = argv[1];
    save_num = malloc(strlen(num) + 1);

    signal(SIGALRM, sig_alrm);
    signal(SIGINT, sig_int);

    alarm(SEC_TO_ALARM);

    while (*num != '0') {
        num_to_print = num;    
        save_num = strcpy(save_num, num);
        num_to_print = save_num;        
        num = dec(num);
    }

    free(save_num);
    return 0;
}


/*
    Задача: 
        Получить ооогромное число и уменьшить его до 0.
        Чтобы число было ооогромным оно представлено в виде строки.
        Чтобы уменьшить число на 1 используется функция dec, 
        ее реализация в ejudge.
        А также нужно каждые 5 секунд 
        выводить результат вычислений.
        Кроме того, по сигналу SIGINT 
        нужно распечатать кол-во секунд до вывода значения.

    Реализация:
        Для просмотра промежуточных значений числа 
        используется SIGALRM
        Процесс получает его каждые 5 секунд 
        (он вызывается рекурсивно см. sig_alrm).

        Чтобы посмотреть сколько времени осталось 
        до вывода промежуточного значения,
        установлен обработчик для сигнала SIGINT.
        Обработчик сбрасывает текущий вызов alarm, вызывая alarm(0)
        и печатает значение alarm(0), 
        т.е. сколько оставалось времени до "нового" SIGALRM.
        После этого перезапускает "рекурсию", 
        вызывая alarm от того числа, которое распечатали.
        На самом деле это немного тормозит 
        вывод промежуточных значений:
            Все округляется до секунд. 
            К примеру, получив SIGINT 
            за 1.0001 секунду до нового SIGALRM, 
            процесс получит новый SIGALRM после 2 секунд,
            потому что округление идет в большую сторону 
            (см. реализацию alarm)
            Но на самом деле даже больше, 
            так как нужно отработать системному вызову alarm(0), 
            а потом системному вызову alarm от 
            полученного alarm(0) числа.

    Проблемы и её решение:
            Так как число может быть большим, 
            то во время выполнения dec может прийти SIGALRM.
            Что тогда?
                1.1) Пропустить этот сигнал 
                (не подходит под условие задачи)
                1.2) Распечатать число в процессе преобразования 
                (плохое решение)
                1.3) Подождать немного, потом распечатать 
                (непонятно сколько ждать, 
                т.к. неизвестна реализация dec, размер числа)
                1.4) Распечатать число сразу после 
                завершения преобразования
                (не подходит под условие задачи)
                1.5) Сохранять значение числа перед вычислением 
                и печатать его.

            Я выбрал вариант 1.5 как самый надежный и правильный.

            Чтобы реализовать 1.5 пришлось использовать 
            глобальную переменную-строку - копию исходного числа.
*/