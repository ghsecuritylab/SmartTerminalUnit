﻿/*
 *  mms_server.h
 *
 *  Copyright 2013, 2014 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#ifndef MMS_SERVER_H_
#define MMS_SERVER_H_

/** \addtogroup mms_server_api_group
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "mms_device_model.h"
#include "mms_value.h"
#include "iso_server.h"

typedef enum {
	MMS_SERVER_NEW_CONNECTION, MMS_SERVER_CONNECTION_CLOSED
} MmsServerEvent;

typedef struct sMmsServer* MmsServer;

typedef struct sMmsServerConnection* MmsServerConnection;

typedef enum {
    MMS_DOMAIN_SPECIFIC,
    MMS_ASSOCIATION_SPECIFIC,
    MMS_VMD_SPECIFIC
} MmsVariableListType;

typedef MmsValue* (*MmsReadVariableHandler)(void* parameter, MmsDomain* domain,
		char* variableId, MmsServerConnection connection);

typedef MmsDataAccessError (*MmsReadAccessHandler) (void* parameter, MmsDomain* domain,
        char* variableId, MmsServerConnection connection);

typedef MmsDataAccessError (*MmsWriteVariableHandler)(void* parameter,
		MmsDomain* domain, char* variableId, MmsValue* value,
		MmsServerConnection connection);

typedef void (*MmsConnectionHandler)(void* parameter,
		MmsServerConnection connection, MmsServerEvent event);

MmsServer
MmsServer_create(MmsDevice* device, TLSConfiguration tlsConfiguration);

void
MmsServer_destroy(MmsServer self);

void
MmsServer_installReadHandler(MmsServer self, MmsReadVariableHandler,
		void* parameter);

void
MmsServer_installReadAccessHandler(MmsServer self, MmsReadAccessHandler, void* parameter);

void
MmsServer_installWriteHandler(MmsServer self, MmsWriteVariableHandler,
		void* parameter);

void
MmsServer_setLocalIpAddress(MmsServer self, const char* localIpAddress);

bool
MmsServer_isRunning(MmsServer self);

/**
 * A connection handler will be invoked whenever a new client connection is opened or closed
 */
void
MmsServer_installConnectionHandler(MmsServer self, MmsConnectionHandler,
		void* parameter);

void
MmsServer_setClientAuthenticator(MmsServer self, AcseAuthenticator authenticator, void* authenticatorParameter);

MmsDevice*
MmsServer_getDevice(MmsServer self);

MmsValue*
MmsServer_getValueFromCache(MmsServer self, MmsDomain* domain, const char* itemId);

bool
MmsServer_isLocked(MmsServer self);

/**
 * \brief callback handler that is called whenever a named variable list changes
 *
 * \param parameter a user provided parameter
 * \param create if true the the request if a request to create a new variable list, false is a delete request
 * \param listType the type (scope) of the named variable list (either domain, association or VMD specific)
 * \param domain the MMS domain the list is belonging to (is NULL for association or VMD specific lists!)
 * \param listName the name
 * \param connection client connection that requests the creation of deletion of the variable list
 *
 * \return MMS_ERROR_NONE if the request is accepted, otherwise the MmsError value that has to be sent back to the client
 */
typedef MmsError (*MmsNamedVariableListChangedHandler)(void* parameter, bool create, MmsVariableListType listType, MmsDomain* domain,
        char* listName, MmsServerConnection connection);

/**
 * \brief Install callback handler that is called when a named variable list changes (is created or deleted)
 *
 * \param self the MmsServer instance to operate on
 * \param handler the callback handler function
 * \param parameter user provided parameter that is passed to the callback handler
 */
void
MmsServer_installVariableListChangedHandler(MmsServer self, MmsNamedVariableListChangedHandler handler, void* parameter);

/**
 * \brief ObtainFile service callback handler
 *
 * This is invoked when the obtainFile service is requested by the client. It can be used to accept or deny the
 * write access to the file system based on the client connection.
 *
 * \param parameter user provided parameter that is passed to the callback handler
 * \param connection the connection that requested the service
 * \param sourceFilename the source file name on the client side system
 * \param destinationFilename the target file name on the server side system
 */
typedef bool (*MmsObtainFileHandler)(void* parameter, MmsServerConnection connection, const char* sourceFilename, const char* destinationFilename);

/**
 * \brief Install callback handler that is invoked when the file upload (obtainFile service) is invoked by the client
 *
 * This handler can be used to apply access restrictions on the file upload (obtainFile) service
 *
 * \param self the MmsServer instance to operate on
 * \param handler the callback handler function
 * \param parameter user provided parameter that is passed to the callback handler
 */
void
MmsServer_installObtainFileHandler(MmsServer self, MmsObtainFileHandler handler, void* parameter);

/**
 * \brief Get file complete (obtainFile service terminated) callback handler
 *
 * This handler can be used to invoke some actions after a file upload is complete.
 *
 * \param parameter user provided parameter that is passed to the callback handler
 * \param connection the connection that requested the service
 * \param destinationFilename the target file name on the server side system
 */
typedef void (*MmsGetFileCompleteHandler)(void* parameter, MmsServerConnection connection, const char* destinationFilename);

/**
 * \brief Install callback handler that is invoked when the file upload (obtainFile service) is completed and the
 *        file has been uploaded.
 *
 * \param self the MmsServer instance
 * \param handler the callback handler function
 * \param parameter user provided parameter that is passed to the callback handler
 */
void
MmsServer_installGetFileCompleteHandler(MmsServer self, MmsGetFileCompleteHandler handler, void* parameter);


typedef  enum {
    MMS_FILE_ACCESS_TYPE_READ_DIRECTORY,
    MMS_FILE_ACCESS_TYPE_OPEN,
    MMS_FILE_ACCESS_TYPE_OBTAIN,
    MMS_FILE_ACCESS_TYPE_DELETE,
    MMS_FILE_ACCESS_TYPE_RENAME
} MmsFileServiceType;

/**
 * \brief MmsFileAccessHandler callback function. Use to monitor and control file access
 *
 * \param parameter user provided parameter that is passed to the callback handler
 * \param connection the connection that requested the service
 * \param service the requested file service
 * \param localFilename the requested file or directory name at the server
 * \param otherFilename a second file name parameter (e.g. source file of the ObtainFile or new file of rename file)
 *
 * \return MMS_ERROR_NONE when the request is accepted, otherwise use the appropriate error code (e.g. MMS_ERROR_FILE_FILE_ACCESS_DENIED)
 */
typedef MmsError (*MmsFileAccessHandler) (void* parameter, MmsServerConnection connection, MmsFileServiceType service,
                                          const char* localFilename, const char* otherFilename);


/**
 * \brief Install a callback handler this is invoked when the client requests a file server. This function can be
 *        used to monitor and control file access
 *
 * \param self the MmsServer instance
 * \param handler the callback handler function
 * \param parameter user provided parameter that is passed to the callback handler
 */
void
MmsServer_installFileAccessHandler(MmsServer self, MmsFileAccessHandler handler, void* parameter);

/**
 * \brief Set the virtual filestore basepath for the MMS file services
 *
 * All external file service accesses will be mapped to paths relative to the base directory.
 * NOTE: This function is only available when the CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME
 * option in stack_config.h is set.
 *
 * \param self the MmsServer instance
 * \param basepath the new virtual filestore basepath
 */
void
MmsServer_setFilestoreBasepath(MmsServer self, const char* basepath);

/**
 * \brief lock the cached server data model
 *
 * NOTE: This method should never be called inside of a library callback function. In the context of
 * a library callback the data model is always already locked! Calling this function inside of a
 * library callback may lead to a deadlock condition.
 *
 * \param self the MmsServer instance to operate on
 */
void
MmsServer_lockModel(MmsServer self);

/**
 * \brief unlock the cached server data model
 *
 * NOTE: This method should never be called inside of a library callback function. In the context of
 * a library callback the data model is always already locked!
 *
 * \param self the MmsServer instance to operate on
 */
void
MmsServer_unlockModel(MmsServer self);

void
MmsServer_insertIntoCache(MmsServer self, MmsDomain* domain, char* itemId,
		MmsValue* value);

/***************************************************
 * Functions for multi-threaded operation mode
 ***************************************************/

/**
 * \brief Start a new server thread and listen for incoming connections
 *
 * \param self the MmsServer instance to operate on
 * \param tcpPort the TCP port the server is listening on.
 */
void
MmsServer_startListening(MmsServer self, int tcpPort);

/**
 * \brief Stop server thread an all open connection threads
 *
 * \param self the MmsServer instance to operate on
 */
void
MmsServer_stopListening(MmsServer self);

/***************************************************
 * Functions for threadless operation mode
 ***************************************************/

/**
 * \brief Start a new server in non-threaded operation mode
 *
 * \param self the MmsServer instance to operate on
 * \param tcpPort the TCP port the server is listening on.
 */
void
MmsServer_startListeningThreadless(MmsServer self, int tcpPort);

/**
 * \brief Wait for the server to come ready in non-threaded operation mode
 *
 * \param self the MmsServer instance to operate on
 * \param timeoutMs maximum number of milliseconds to wait
 * \return 1 if the server is ready, 0 if not or -1 on error
 */
int
MmsServer_waitReady(MmsServer self, unsigned int timeoutMs);

/**
 * \brief Handle client connections (for non-threaded operation mode)
 *
 * This function is listening for new client connections and handles incoming
 * requests for existing client connections.
 *
 * \param self the MmsServer instance to operate on
 */
void
MmsServer_handleIncomingMessages(MmsServer self);

/**
 * \brief Handle MmsServer background task
 *
 * \param self the MmsServer instance to operate on
 */
void
MmsServer_handleBackgroundTasks(MmsServer self);

/**
 * \brief Stop the server (for non-threaded operation mode)
 *
 * \param self the MmsServer instance to operate on
 */
void
MmsServer_stopListeningThreadless(MmsServer self);


/***************************************************
 * Functions for MMS identify service
 ***************************************************/

/**
 * \brief set the values that the server will give as response for an MMS identify request
 *
 * With this function the VMD identity attributes can be set programmatically. If not set by this
 * function the default values form stack_config.h are used.
 *
 * \param self the MmsServer instance to operate on
 * \param vendorName the vendor name attribute of the VMD
 * \param modelName the model name attribute of the VMD
 * \param revision the revision attribute of the VMD
 */
void
MmsServer_setServerIdentity(MmsServer self, char* vendorName, char* modelName, char* revision);

/**
 * \brief get the vendor name attribute of the VMD identity
 *
 * \param self the MmsServer instance to operate on
 * \return the vendor name attribute of the VMD as C string
 */
char*
MmsServer_getVendorName(MmsServer self);

/**
 * \brief get the model name attribute of the VMD identity
 *
 * \param self the MmsServer instance to operate on
 * \return the model name attribute of the VMD as C string
 */
char*
MmsServer_getModelName(MmsServer self);

/**
 * \brief get the revision attribute of the VMD identity
 *
 * \param self the MmsServer instance to operate on
 * \return the revision attribute of the VMD as C string
 */
char*
MmsServer_getRevision(MmsServer self);

/***************************************************
 * Functions for MMS status service
 ***************************************************/

#define MMS_LOGICAL_STATE_STATE_CHANGES_ALLOWED 0
#define MMS_LOGICAL_STATE_NO_STATE_CHANGES_ALLOWED 1
#define MMS_LOGICAL_STATE_LIMITED_SERVICES_PERMITTED 2
#define MMS_LOGICAL_STATE_SUPPORT_SERVICES_ALLOWED 3

#define MMS_PHYSICAL_STATE_OPERATIONAL 0
#define MMS_PHYSICAL_STATE_PARTIALLY_OPERATIONAL 1
#define MMS_PHYSICAL_STATE_INOPERATIONAL 2
#define MMS_PHYSICAL_STATE_NEEDS_COMMISSIONING 3

/**
 * \brief User provided handler that is invoked on a MMS status request
 *
 * The extendedDerivation parameter indicates that the client requests the server to perform
 * self diagnosis tests before answering the request.
 *
 * \param parameter is a user provided parameter
 * \param mmsServer is the MmsServer instance
 * \param connection is the MmsServerConnection instance that received the MMS status request
 * \param extendedDerivation indicates if the request contains the extendedDerivation parameter
 */
typedef void (*MmsStatusRequestListener)(void* parameter, MmsServer mmsServer, MmsServerConnection connection, bool extendedDerivation);

/**
 * \brief set the VMD state values for the VMD status service
 *
 * \param self the MmsServer instance to operate on
 * \param vmdLogicalStatus the logical status attribute of the VMD
 * \param vmdPhysicalStatus the physical status attribute of the VMD
 */
void
MmsServer_setVMDStatus(MmsServer self, int vmdLogicalStatus, int vmdPhysicalStatus);

/**
 * \brief get the logical status attribute of the VMD
 *
 * \param self the MmsServer instance to operate on
 */
int
MmsServer_getVMDLogicalStatus(MmsServer self);

/**
 * \brief get the physical status attribute of the VMD
 *
 * \param self the MmsServer instance to operate on
 */
int
MmsServer_getVMDPhysicalStatus(MmsServer self);

/**
 * \brief set the MmsStatusRequestListener for this MmsServer
 *
 * With this function the API user can register a listener that is invoked whenever a
 * MMS status request is received from a client. Inside of the handler the user can
 * provide new status values with the MmsServer_setVMDStatus function.
 *
 * \param self the MmsServer instance to operate on
 * \param listener the listener that is called when a MMS status request is received
 * \param parameter a user provided parameter that is handed over to the listener
 */
void
MmsServer_setStatusRequestListener(MmsServer self, MmsStatusRequestListener listener, void* parameter);

char*
MmsServerConnection_getClientAddress(MmsServerConnection self);

IsoConnection
MmsServerConnection_getIsoConnection(MmsServerConnection self);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MMS_SERVER_H_ */
