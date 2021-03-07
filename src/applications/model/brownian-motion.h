/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Georgia Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Siddharth Gangadhar <lordlabakdas@protonmail.com>
 */


#ifndef BROWNIAN_MOTION_H
#define BROWNIAN_MOTION_H

#include "ns3/address.h"
#include "ns3/application.h"

namespace ns3{

	class Address;
	class Socket;

	class BrownianMotion : public Application{
	public:
	  /**
	   * \brief Get the type ID.
	   * \return the object TypeId
	   */
	  static TypeId GetTypeId (void);

	  BrownianMotion ();

	  virtual ~BrownianMotion ();

	protected:
	  virtual void DoDispose (void);
	private:
	  virtual void StartApplication (void);
	  virtual void StopApplication (void);

	};

}
