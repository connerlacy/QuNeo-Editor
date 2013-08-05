#ifndef LEFTRIGHTEDITPANE_H
#define LEFTRIGHTEDITPANE_H

#include <QObject>
#include <QWidget>
#include <QtGui>
#include "leftrightlabels.h"

class LeftrightEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit LeftrightEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *mainWindowAddress, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QLabel *paneLabel;
    QWidget *focusCheck;
    QDial *sensitivityDial;
    QCheckBox *enableBank;
    QWidget *clicked;
    QString prevObjectToLabel;
    QComboBox *modeMenuL;
    QComboBox *modeMenuR;
    QComboBox *bankControl;

    void findUiElements();
    void displayLeftrightValues();

    int currentPreset;
    int currentLeftright;

    QVariantMap leftrightMap;
    QStringList leftrightParamList; //list of parameter names
    QStringList leftrightUiBlacklist;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in leftrightParamList
    
    LeftrightLabels *leftrightLabel[4];

    //pointers for velocity check boxes
    QCheckBox *leftrightLVelocity;
    QCheckBox *leftrightRVelocity;
    QCheckBox *localLEDControl;

    QSpinBox *ledInputChannel;

signals:
    void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void signalToLabels(QString);
    
public slots:
    void slotEvents(QString);
    void slotRecallPreset(int);
    void slotValueChanged(int);
    void slotEnable(int);
    void slotToLabels(QString);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
};

#endif // LEFTRIGHTEDITPANE_H
