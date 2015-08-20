/*
 * Copyright (c) 2012-2013 NEC Corporation
 * All rights reserved.
 * 
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this
 * distribution, and is available at http://www.eclipse.org/legal/epl-v10.html
 */

package org.opendaylight.vtn.core.ipc;

/**
 * <p>
 *   The {@code IpcAlreadyExistsException} class is an exception to notify that
 *   the specified IPC resource already exists.
 *   Typically <strong>EEXIST</strong> error returned by the IPC framework
 *   library is reported by this exception.
 * </p>
 *
 * @since	C10
 */
public class IpcAlreadyExistsException extends IpcException
{
	final static long  serialVersionUID = -4056348970459720275L;

	/**
	 * Construct an {@code IpcAlreadyExistsException} without detailed
	 * message.
	 */
	public IpcAlreadyExistsException()
	{
		super();
	}

	/**
	 * Construct an {@code IpcAlreadyExistsException} with the specified
	 * detailed message.
	 *
	 * @param message	The detailed message.
	 */
	public IpcAlreadyExistsException(String message)
	{
		super(message);
	}

	/**
	 * Construct an {@code IpcAlreadyExistsException} with the specified
	 * detailed message and cause.
	 *
	 * @param message	The detailed message.
	 * @param cause		The cause of this exception.
	 */
	public IpcAlreadyExistsException(String message, Throwable cause)
	{
		super(message, cause);
	}

	/**
	 * Construct an {@code IpcAlreadyExistsException} with the specified
	 * cause.
	 *
	 * @param cause		The cause of this exception.
	 */
	public IpcAlreadyExistsException(Throwable cause)
	{
		super(cause);
	}
}
