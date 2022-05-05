#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#endif
#include "time.h"

#include <ESP_Mail_Client.h>
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


const char* ntpServer = "pool.ntp.org";

unsigned long timestamp;
#define IMAP_HOST "<host>"

/** The imap port e.g. 
 * 143  or esp_mail_imap_port_143
 * 993 or esp_mail_imap_port_993
*/
#define IMAP_PORT 993

/* The log in credentials */
#define AUTHOR_EMAIL "rewardbox2022@gmail.com"
#define AUTHOR_PASSWORD "vsdbmb2312"

/* Callback function to get the Email reading status */
void imapCallback(IMAP_Status status);

/* Print the list of mailbox folders */
void printAllMailboxesInfo(IMAPSession &imap);

/* Print the selected folder info */
void printSelectedMailboxInfo(SelectedFolderInfo sFolder);

/* Print all messages from the message list */
void printMessages(std::vector<IMAP_MSG_Item> &msgItems, bool headerOnly);

/* Print all attachments info from the message */
void printAttacements(std::vector<IMAP_Attach_Item> &atts);

/* The IMAP Session object used for Email reading */
IMAPSession imap;

String message;
int timeofmessage=0;
String BTdata;
String WIFIusername;
String WIFIpassword;
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    
  }
  time(&now);
  return now;
}

void setup()
{
    pinMode(12,OUTPUT);
    Serial.begin(115200);
    SerialBT.begin();
    Serial.println("Bluetooth Started! Ready to pair...");
    SerialBT.println("Type your Wifi Network and then press enter and type in your Wifi Password");
    int n = WiFi.scanNetworks();
    if (SerialBT.available()){
      BTdata=SerialBT.read();
      for(int i = 0; i < n; i++) {
          if (BTdata==WiFi.SSID(i)){
            WIFIusername=BTdata;
            }
          else{
            WIFIpassword=BTdata;
            }
        }
        #define WIFI_SSID WIFIusername
        #define WIFI_PASSWORD WIFIpassword
    }
    else{
      #define WIFI_SSID "BBS-Student"
      #define WIFI_PASSWORD "gracetoyou"
      }


#if defined(ARDUINO_ARCH_SAMD)
    while (!Serial)
        ;
    Serial.println();
    Serial.println("**** Custom built WiFiNINA firmware need to be installed.****\nTo install firmware, read the instruction here, https://github.com/mobizt/ESP-Mail-Client#install-custom-built-wifinina-firmware");

#endif

    Serial.println();

    Serial.print("Connecting to AP");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(200);
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();



    imap.debug(1);

    /* Set the callback function to get the reading results */
    imap.callback(imapCallback);

    
    ESP_Mail_Session session;

    /* Set the session config */
    session.server.host_name = "imap.gmail.com"; 
    session.server.port = 993;
    session.login.email = "rewardbox2022@gmail.com";
    session.login.password = "vsdbmb2312";

    /* Setup the configuration for searching or fetching operation and its result */
    IMAP_Config config;

    /* Set the storage to save the downloaded files and attachments */
    config.storage.saved_path = F("/email_data");

    /** The file storage type e.g.
     * esp_mail_file_storage_type_none,
     * esp_mail_file_storage_type_flash, and 
     * esp_mail_file_storage_type_sd 
    */
    config.storage.type = esp_mail_file_storage_type_sd;

    /** Set to download heades, text and html messaeges, 
     * attachments and inline images respectively.
    */
    config.download.header = true;
    config.download.text = true;
    config.download.html = true;
    config.download.attachment = true;
    config.download.inlineImg = true;

    /** Set to enable the results i.e. html and text messaeges 
     * which the content stored in the IMAPSession object is limited
     * by the option config.limit.msg_size.
     * The whole message can be download through config.download.text
     * or config.download.html which not depends on these enable options.
    */
    config.enable.html = true;
    config.enable.text = true;

    /* Set to enable the sort the result by message UID in the ascending order */
    config.enable.recent_sort = true;

    /* Set to report the download progress via the default serial port */
    config.enable.download_status = true;



    /* Set the limit of number of messages in the search results */
    config.limit.search = 5;

    /** Set the maximum size of message stored in 
     * IMAPSession object in byte
    */
    config.limit.msg_size = 512;


    config.limit.attachment_size = 1024 * 1024 * 5;


    /* Connect to server with the session and config */
    if (!imap.connect(&session, &config))
        return;

    /*  {Optional} */
    printAllMailboxesInfo(imap);

    /* Open or select the mailbox folder to read or search the message */
    if (!imap.selectFolder(F("INBOX")))
        return;

    /*  {Optional} */
    printSelectedMailboxInfo(imap.selectedFolder());

    /** Message UID to fetch or read e.g. 100. 
     * In this case we will get the UID from the max message number (lastest message) 
    */
    config.fetch.uid = imap.getUID(imap.selectedFolder().msgCount());

   
    imap.empty();
    configTime(0, 0, ntpServer);
}

void loop()
{
  timestamp=getTime();
  delay(1000);
  if (timestamp-timeofmessage<=600 && message=="True"){
      digitalWrite(12,HIGH);
    }
  else{
      digitalWrite(12,LOW);
    }
}

/* Callback function to get the Email reading status */
void imapCallback(IMAP_Status status)
{
    /* Print the current status */
    Serial.println(status.info());

    /* Show the result when reading finished */
    if (status.success())
    {
        /* Print the result */
        /* Get the message list from the message list data */
        IMAP_MSG_List msgList = imap.data();
        printMessages(msgList.msgItems, imap.headerOnly());

        /* Clear all stored data in IMAPSession object */
        imap.empty();
    }
}

void printAllMailboxesInfo(IMAPSession &imap)
{
    /* Declare the folder collection class to get the list of mailbox folders */
    FoldersCollection folders;

    /* Get the mailbox folders */
    if (imap.getFolders(folders))
    {
        for (size_t i = 0; i < folders.size(); i++)
        {
            /* Iterate each folder info using the  folder info item data */
            FolderInfo folderInfo = folders.info(i);
            ESP_MAIL_PRINTF("%s%s%s", i == 0 ? "\nAvailable folders: " : ", ", folderInfo.name, i == folders.size() - 1 ? "\n" : "");
        }
    }
}

void printSelectedMailboxInfo(SelectedFolderInfo sFolder)
{
    /* Show the mailbox info */
    ESP_MAIL_PRINTF("\nInfo of the selected folder\nTotal Messages: %d\n", sFolder.msgCount());
    ESP_MAIL_PRINTF("Predicted next UID: %d\n", sFolder.nextUID());
    ESP_MAIL_PRINTF("Unseen Message Index: %d\n", sFolder.unseenIndex());
    for (size_t i = 0; i < sFolder.flagCount(); i++)
        ESP_MAIL_PRINTF("%s%s%s", i == 0 ? "Flags: " : ", ", sFolder.flag(i).c_str(), i == sFolder.flagCount() - 1 ? "\n" : "");
}

void printAttacements(std::vector<IMAP_Attach_Item> &atts)
{
    ESP_MAIL_PRINTF("Attachment: %d file(s)\n****************************\n", atts.size());
    for (size_t j = 0; j < atts.size(); j++)
    {
        IMAP_Attach_Item att = atts[j];
        /** att.type can be
         * esp_mail_att_type_none or 0
         * esp_mail_att_type_attachment or 1
         * esp_mail_att_type_inline or 2
        */
        ESP_MAIL_PRINTF("%d. Filename: %s, Name: %s, Size: %d, MIME: %s, Type: %s, Creation Date: %s\n", j + 1, att.filename, att.name, att.size, att.mime, att.type == esp_mail_att_type_attachment ? "attachment" : "inline", att.creationDate);
    }
    Serial.println();
}

void printMessages(std::vector<IMAP_MSG_Item> &msgItems, bool headerOnly)
{

    //Serial.println(imap.fileList());

    for (size_t i = 0; i < msgItems.size(); i++)
    {

        /* Iterate to get each message data through the message item data */
        IMAP_MSG_Item msg = msgItems[i];

        Serial.println("****************************");
        ESP_MAIL_PRINTF("Number: %d\n", msg.msgNo);
        ESP_MAIL_PRINTF("UID: %d\n", msg.UID);
        ESP_MAIL_PRINTF("Messsage-ID: %s\n", msg.ID);

        ESP_MAIL_PRINTF("Flags: %s\n", msg.flags);

        //The attachment may not detect in search because the multipart/mixed
        //was not found in Content-Type header field.
        ESP_MAIL_PRINTF("Attachment: %s\n", msg.hasAttachment ? "yes" : "no");

        if (strlen(msg.from))
            ESP_MAIL_PRINTF("From: %s\n", msg.from);
        if (strlen(msg.sender))
            ESP_MAIL_PRINTF("Sender: %s\n", msg.sender);
        if (strlen(msg.to))
            ESP_MAIL_PRINTF("To: %s\n", msg.to);
        if (strlen(msg.cc))
            ESP_MAIL_PRINTF("CC: %s\n", msg.cc);
        if (strlen(msg.date))
        {
            ESP_MAIL_PRINTF("Date: %s\n", msg.date);
            ESP_MAIL_PRINTF("Timestamp: %d\n", (int)MailClient.Time.getTimestamp(msg.date,true));
            timeofmessage=MailClient.Time.getTimestamp(msg.date,true);
        }
        if (strlen(msg.subject))
            ESP_MAIL_PRINTF("Subject: %s\n", msg.subject);
        if (strlen(msg.reply_to))
            ESP_MAIL_PRINTF("Reply-To: %s\n", msg.reply_to);
        if (strlen(msg.return_path))
            ESP_MAIL_PRINTF("Return-Path: %s\n", msg.return_path);
        if (strlen(msg.in_reply_to))
            ESP_MAIL_PRINTF("In-Reply-To: %s\n", msg.in_reply_to);
        if (strlen(msg.references))
            ESP_MAIL_PRINTF("References: %s\n", msg.references);
        if (strlen(msg.comments))
            ESP_MAIL_PRINTF("Comments: %s\n", msg.comments);
        if (strlen(msg.keywords))
            ESP_MAIL_PRINTF("Keywords: %s\n", msg.keywords);

        /* If the result contains the message info (Fetch mode) */
        if (!headerOnly)
        {
            if (strlen(msg.text.content))
                ESP_MAIL_PRINTF("Text Message: %s\n", msg.text.content);
                message=msg.text.content;   
            if (strlen(msg.text.charSet))
                ESP_MAIL_PRINTF("Text Message Charset: %s\n", msg.text.charSet);
            if (strlen(msg.text.transfer_encoding))
                ESP_MAIL_PRINTF("Text Message Transfer Encoding: %s\n", msg.text.transfer_encoding);

            if (msg.rfc822.size() > 0)
            {
                ESP_MAIL_PRINTF("\r\nRFC822 Messages: %d message(s)\n****************************\n", msg.rfc822.size());
                printMessages(msg.rfc822, headerOnly);
            }
        }

       
    }
     Serial.println();
    }
