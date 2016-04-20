/* Copyright 2016 Mikhail Kita
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "sensorSettingsWidget.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QtGui/QScrollArea>
#else
	#include <QtWidgets/QScrollArea>
#endif

#include <QtGui/QKeyEvent>
#include <trikKernel/paths.h>
#include <QsLog.h>

using namespace trikGui;
using trikControl::MotorInterface;

SensorSettingsWidget::SensorSettingsWidget(const QString &port, QWidget *parent)
	: TrikGuiDialog(parent)
	, mPort(port)
{
	QLabel* const powerLabel = new QLabel(tr("Select distance:") + ": \n");
	powerLabel->setAlignment(Qt::AlignTop);
	powerLabel->setMaximumHeight(20);
	mLayout.addWidget(powerLabel);

	mLever = new SensorLever(mPort, this);
	mLever->setMaximumHeight(50);
	mLayout.addWidget(mLever);

	mContinueButton.setText(tr("Continue"));
	mContinueButton.setAutoFillBackground(true);
	mLayout.addWidget(&mContinueButton);

	setLayout(&mLayout);
}

SensorSettingsWidget::~SensorSettingsWidget()
{
	delete(mLever);
}

QString SensorSettingsWidget::menuEntry()
{
	return QString(tr("Sensor Settings"));
}

void SensorSettingsWidget::renewFocus()
{
	const QColor buttonColor = QPalette().color(QPalette::Background);
	QPalette palette;
	palette.setColor(QPalette::Background, buttonColor);
	palette.setColor(QPalette::Base, buttonColor);
	palette.setColor(QPalette::Button, buttonColor);

	mContinueButton.setPalette(palette);
}

void SensorSettingsWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Return: {
		mContinueButton.animateClick();
		event->accept();
		exit();
		break;
	}
	default: {
		TrikGuiDialog::keyPressEvent(event);
		break;
	}
	}
}

QString SensorSettingsWidget::createScript()
{
	QString sign(">");
	if (!mLever->isGrater()) {
		sign = "<";
	}
	return QString("    while (!(brick.sensor(%1).read() %2 %3)) {\n"
		"        script.wait(10);\n    }\n").arg(mPort).arg(sign).arg(mLever->distance());
}
