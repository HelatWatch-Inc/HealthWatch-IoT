#include "declarations.h"
#include "draw.h"
#include "bitmaps.h"

// Auxiliares

int getTextWidth(String text)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    return w;
}

void displayTextWrapped(String text, int x, int y, int maxWidth)
{
    int cursorX = x;
    int cursorY = y;

    // Separa el texto en palabras
    int start = 0;
    int end = text.indexOf(' ', start);

    while (end != -1)
    {
        String word = text.substring(start, end + 1);
        int wordWidth = display.width() - cursorX;
        int wordWidthPixels = getTextWidth(word);

        if (wordWidthPixels > wordWidth)
        {
            // Si la palabra no cabe, mover a la siguiente línea
            cursorX = x;
            cursorY += 10; // Desplazamiento en Y (altura de la línea) puede necesitar ajuste
        }

        if (cursorY >= 50)
        {
            display.setCursor(17, 50);
            display.print("... continua ...");
            return;
        }
        // Dibujar la palabra
        display.setCursor(cursorX, cursorY);
        display.print(word);
        cursorX += wordWidthPixels;

        // Ir a la siguiente palabra
        start = end + 1;
        end = text.indexOf(' ', start);
    }

    // Imprimir la última palabra
    String lastWord = text.substring(start);
    display.setCursor(cursorX, cursorY);
    display.print(lastWord);
}

void drawCruz(int x, int y)
{
    display.drawBitmap(x, y, image_cruz_bits, 3, 3, 1);
}

void drawFrame()
{
    drawCruz(2, 2);
    drawCruz(123, 2);
    drawCruz(2, 16);
    drawCruz(123, 16);
    drawCruz(2, 59);
    drawCruz(123, 59);
    display.drawLine(6, 17, 121, 17, 1);
    display.drawLine(3, 6, 3, 14, 1);
    display.drawLine(124, 6, 124, 14, 1);
    display.drawLine(6, 3, 121, 3, 1);
    display.drawLine(3, 20, 3, 57, 1);
    display.drawLine(124, 20, 124, 57, 1);
    display.drawLine(6, 60, 121, 60, 1);
}

void drawRectMenu()
{
    display.drawRoundRect(2, 2, 119, 18, 2, 1);
    display.drawRoundRect(2, 22, 119, 20, 2, 1);
    display.drawRect(3, 23, 117, 18, 1);
    display.drawRoundRect(2, 44, 119, 18, 2, 1);
}

void centerText(String text, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int x = (SCREEN_WIDTH - w) / 2;
    display.setCursor(x, y);
    display.print(text);
}

// Pantallas

void drawClock(bool fullRedraw)
{
    ntpClient.update();
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        drawFrame();
        // Dibujo de clima
        display.drawBitmap(26, 47, image_cloud_bits, 17, 16, 1);
        display.setCursor(46, 49);
        display.print("Clima:");
        display.setCursor(96, 49);
        display.print("C");
        // Dibujo de fecha
        unsigned long epochTime = ntpClient.getEpochTime();
        struct tm *ptm = gmtime((time_t *)&epochTime);
        int day = ptm->tm_mday;
        int month = ptm->tm_mon;
        int year = ptm->tm_year + 1900;
        int weekday = ptm->tm_wday;
        display.setCursor(26, 7);
        display.printf("%s %02d-%s-%02d", daysOfTheWeek[weekday], day, monthsOfTheYear[month], year % 100);
    }
    display.setTextSize(1);
    // Dibujo de temperatura
    display.fillRect(83, 49, 12, 8, BLACK);
    display.setCursor(83, 49);
    display.print("28");
    // Dibujo de hora
    display.setTextSize(2);
    display.fillRect(16, 27, 98, 16, BLACK);
    display.setCursor(17, 27);
    display.print(ntpClient.getFormattedTime());
    display.display();
}

void drawMenu(bool fullRedraw, int items[])
{
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        drawRectMenu();
    }
    // dibujar barra
    display.fillRect(123, 0, 3, 64, BLACK);
    display.drawLine(124, 2, 124, 61, 1);
    display.drawRect(123, 64 / NUM_ITEMS * items[ITEM_PREVIUS] + 1, 3, 64 / NUM_ITEMS, 1);
    // dibujar item previo
    display.fillRect(4, 4, 114, 15, BLACK);
    display.drawBitmap(6, 5, icons_bitmaps[items[0]], 17, 12, 1);
    display.setCursor(27, 7);
    display.print(menuItems[items[0]]);
    // dibujar item seleccionado
    display.fillRect(4, 24, 114, 16, BLACK);
    display.drawBitmap(6, 26, icons_bitmaps[items[1]], 17, 12, 1);
    display.setCursor(27, 28);
    display.print(menuItems[items[1]]);
    // dibujar item posterior
    display.fillRect(4, 46, 114, 15, BLACK);
    display.drawBitmap(6, 47, icons_bitmaps[items[2]], 17, 12, 1);
    display.setCursor(27, 49);
    display.print(menuItems[items[2]]);
    display.display();
}

// Items

void drawChronometer(bool fullRedraw, unsigned long elapsedTime)
{
    if (fullRedraw)
    {
        display.clearDisplay();
        drawFrame();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(35, 7);
        display.print("Cronometro");
        // pausa
        display.drawBitmap(35, 40, image_icon_pausa_bits, 11, 11, 1);
        display.drawBitmap(31, 53, str_pausa_bits, 19, 5, 1);
        // exit
        display.drawBitmap(59, 40, image_icon_exit_bits, 11, 11, 1);
        display.drawBitmap(55, 53, str_exit_bits, 19, 5, 1);
        // reset
        display.drawBitmap(83, 40, image_icon_reset_bits, 11, 11, 1);
        display.drawBitmap(79, 53, str_reset_bits, 19, 5, 1);
    }
    // Dibujo de la cuenta
    display.fillRect(16, 21, 98, 16, BLACK);
    unsigned int milliseconds = (elapsedTime % 1000) / 10;
    unsigned int seconds = (elapsedTime / 1000) % 60;
    unsigned int minutes = (elapsedTime / 60000) % 60;
    display.setTextSize(2);
    display.setCursor(17, 22);
    display.printf("%02d:%02d:%02d", minutes, seconds, milliseconds);
    display.display();
}
