/* 
 * File:   MidiOutWorker.h
 * Author: jrussell
 *
 * Created on September 11, 2013, 9:15 PM
 */

#ifndef MIDIOUTWORKER_H
#define	MIDIOUTWORKER_H
#include <QThread>
#include <QtCore>
class MidiOutWorker :public QObject {
    Q_OBJECT
public:
    MidiOutWorker(int client, int port); //message?
    MidiOutWorker(const MidiOutWorker& orig);
    virtual ~MidiOutWorker();
signals:
public slots:
    //Do the work
    void work();
private:
};

#endif	/* MIDIOUTWORKER_H */

