/*
 * Copyright (c) 2012-2013 NEC Corporation
 * All rights reserved.
 * 
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this
 * distribution, and is available at http://www.eclipse.org/legal/epl-v10.html
 */

package org.opendaylight.vtn.core.ipc;

import java.net.InetAddress;

/**
 * <p>
 *   The {@code IpcInet6Address} class is an IPC additional data unit class
 *   which represents an IPv6 address.
 * </p>
 * <p>
 *   Note that an {@code IpcInet6Address} instance represents an IPv6 address
 *   only. The scoped network interface in {@code Inet6Address} is never
 *   transferred via IPC connection.
 * </p>
 *
 * @since	C10
 * @see	IpcInetAddress#create(InetAddress)
 */
public final class IpcInet6Address extends IpcInetAddress
{
	/**
	 * Construct an IPv6 address.
	 *
	 * @param value		A value for a new object.
	 */
	IpcInet6Address(InetAddress value)
	{
		super(value);
	}

	/**
	 * Return the type identifier associated with this data.
	 *
	 * @return	{@link IpcDataUnit#IPV6} is always returned.
	 */
	@Override
	public int getType()
	{
		return IPV6;
	}

	/**
	 * Append this data to the end of the additional data in the
	 * specified IPC client session
	 *
	 * @param session	An IPC client session handle.
	 * @param addr		A {@code byte} array which contains raw IP
	 *			address.
	 * @throws IpcResourceBusyException
	 *	Another IPC service request is being issued on the specified
	 *	session.
	 * @throws IpcShutdownException
	 *	The state of the specified session is <strong>DISCARD</strong>.
	 *	This exception indicates that the specified session is no
	 *	longer available.
	 * @throws IpcBadStateException
	 *	The state of the specified session is <strong>RESULT</strong>.
	 * @throws IpcTooBigDataException
	 *	No more additional data can not be added.
	 */
	final native void addClientOutput(long session, byte[] addr)
		throws IpcException;
}
