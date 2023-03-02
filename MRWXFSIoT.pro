#QT -= gui
QT += websockets

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/cpp/

MRW_PROJECT_DIR = E:/source_code/repos/github/Miraway/MirawaySelfService
LIBS += -L$$MRW_PROJECT_DIR\libs
INCLUDEPATH += $$MRW_PROJECT_DIR/include

INCLUDEPATH += $$MRW_PROJECT_DIR/src/MRWQTLogger/cpp_src
LIBS += -lMRWQTLogger

INCLUDEPATH += $$MRW_PROJECT_DIR/src/MRWSSCore/cpp_src/
INCLUDEPATH += $$MRW_PROJECT_DIR/src/MRWSSCore/
LIBS += -lMRWSSCore

#XFS SDK
INCLUDEPATH += $$MRW_PROJECT_DIR/XFS/SDK/INCLUDE
LIBS += -L$$MRW_PROJECT_DIR\XFS\SDK\LIB -lmsxfs

SOURCES += \
        main.cpp \
        service_manager.cpp \
        src/cpp/device/abstract_device_worker.cpp \
        src/cpp/device/device_worker.cpp \
        src/cpp/device/device_worker_thread.cpp \
        src/cpp/device/v3x/device_worker_thread_v3x.cpp \
        src/cpp/device/v3x/device_worker_v3x.cpp \
        src/cpp/event/xfs_iot_event.cpp \
        src/cpp/event/xfs_iot_msg_event.cpp \
        src/cpp/interface/interface_repository.cpp \
        src/cpp/interface/xfs_iot_function.cpp \
        src/cpp/interface/xfs_iot_interface.cpp \
        src/cpp/service/abstract_service.cpp \
        src/cpp/service/client_handle.cpp \
        src/cpp/service/command_queue.cpp \
        src/cpp/service/cardreader/cardreader_service.cpp \
        src/cpp/service/printer/abstract_printer_service.cpp \
        src/cpp/service/printer/bmp_printer.cpp \
        src/cpp/service/printer/form/element/alignment.cpp \
        src/cpp/service/printer/form/element/area.cpp \
        src/cpp/service/printer/form/element/block_element.cpp \
        src/cpp/service/printer/form/element/element.cpp \
        src/cpp/service/printer/form/element/index.cpp \
        src/cpp/service/printer/form/element/number_element.cpp \
        src/cpp/service/printer/form/element/position.cpp \
        src/cpp/service/printer/form/element/size.cpp \
        src/cpp/service/printer/form/element/string_combination_element.cpp \
        src/cpp/service/printer/form/element/string_element.cpp \
        src/cpp/service/printer/form/element/unit.cpp \
        src/cpp/service/printer/form/element/version.cpp \
        src/cpp/service/printer/form/text_line_producer.cpp \
        src/cpp/service/printer/form/xfs_field.cpp \
        src/cpp/service/printer/form/xfs_form.cpp \
        src/cpp/service/printer/form/xfs_form_repository.cpp \
        src/cpp/service/printer/form/xfs_frame.cpp \
        src/cpp/service/printer/form/xfs_media.cpp \
        src/cpp/service/printer/masung/masung_printer_worker.cpp \
        src/cpp/service/printer/printer_service.cpp \
        src/cpp/service/printer/printer_service_v3x.cpp \
        src/cpp/service/printer/printer_worker.cpp \
        src/cpp/service/publisher/publisher_service.cpp \
        src/cpp/service/service_endpoint.cpp \
        src/cpp/service/service_repository.cpp \
        xfs_iot_standard.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#win32:CONFIG(release, debug|release): LIBS +=  -lmsxfs
#else:win32:CONFIG(debug, debug|release): LIBS += -lmsxfs

HEADERS += \
    service_manager.h \
    src/cpp/device/abstract_device_worker.h \
    src/cpp/device/device_worker.h \
    src/cpp/device/device_worker_thread.h \
    src/cpp/device/v3x/device_worker_thread_v3x.h \
    src/cpp/device/v3x/device_worker_v3x.h \
    src/cpp/event/xfs_iot_event.h \
    src/cpp/event/xfs_iot_msg_event.h \
    src/cpp/interface/interface_repository.h \
    src/cpp/interface/xfs_iot_function.h \
    src/cpp/interface/xfs_iot_interface.h \
    src/cpp/service/abstract_service.h \
    src/cpp/service/client_handle.h \
    src/cpp/service/command_queue.h \
    src/cpp/service/cardreader/cardreader_service.h \
    src/cpp/service/printer/abstract_printer_service.h \
    src/cpp/service/printer/bmp_printer.h \
    src/cpp/service/printer/form/element/alignment.h \
    src/cpp/service/printer/form/element/area.h \
    src/cpp/service/printer/form/element/block_element.h \
    src/cpp/service/printer/form/element/element.h \
    src/cpp/service/printer/form/element/index.h \
    src/cpp/service/printer/form/element/number_element.h \
    src/cpp/service/printer/form/element/position.h \
    src/cpp/service/printer/form/element/size.h \
    src/cpp/service/printer/form/element/string_combination_element.h \
    src/cpp/service/printer/form/element/string_element.h \
    src/cpp/service/printer/form/element/unit.h \
    src/cpp/service/printer/form/element/version.h \
    src/cpp/service/printer/form/text_line_producer.h \
    src/cpp/service/printer/form/xfs_field.h \
    src/cpp/service/printer/form/xfs_form.h \
    src/cpp/service/printer/form/xfs_form_repository.h \
    src/cpp/service/printer/form/xfs_frame.h \
    src/cpp/service/printer/form/xfs_media.h \
    src/cpp/service/printer/masung/masung_printer_simulator.h \
    src/cpp/service/printer/masung/masung_printer_worker.h \
    src/cpp/service/printer/masung/sdk/Msprintsdk.h \
    src/cpp/service/printer/masung/sdk/Msqrcode.h \
    src/cpp/service/printer/printer_service.h \
    src/cpp/service/printer/printer_service_v3x.h \
    src/cpp/service/printer/printer_worker.h \
    src/cpp/service/publisher/publisher_service.h \
    src/cpp/service/service_endpoint.h \
    src/cpp/service/service_repository.h \
    xfs_iot_standard.h
