/**
 * Copyright (C) ONERA 2010
 * Version: 1.0
 * Author: Charles Lesire <charles.lesire@onera.fr>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ORO_YARP_BOTTLE_ARCHIVE
#define ORO_YARP_BOTTLE_ARCHIVE

#include <boost/mpl/bool.hpp>
#include <boost/version.hpp>
#include <yarp/os/all.h>
#include <boost/archive/detail/basic_pointer_iserializer.hpp>
#include <boost/archive/detail/basic_pointer_oserializer.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/is_bitwise_serializable.hpp>
#include <boost/archive/detail/iserializer.hpp>
#include <boost/archive/detail/oserializer.hpp>
#include <boost/archive/archive_exception.hpp>

namespace barch = boost::archive;

class yarp_bottle_iarchive {
	yarp::os::Bottle& m_bottle;
public:
	typedef boost::mpl::bool_<true> is_loading;
	typedef boost::mpl::bool_<false> is_saving;

	yarp_bottle_iarchive(yarp::os::Bottle& bottle) :
		m_bottle(bottle) {
	}

	unsigned int get_library_version() {
		return 0;
	}
	void reset_object_address(const void * new_address,
			const void * old_address) {
	}
	void delete_created_pointers() {
	}
	void load_object(void *t, const barch::detail::basic_iserializer & bis) {
		throw barch::archive_exception(
				barch::archive_exception::unregistered_class);
	}
	void load_binary(void *address, std::size_t count) {
		throw barch::archive_exception(
				barch::archive_exception::unregistered_class);
	}

	template<class T>
	const barch::detail::basic_pointer_iserializer * register_type(T * = NULL) {
		return 0;
	}

	template<class T>
	void load_override(T & t, BOOST_PFTO int) {
		load_a_type(t, boost::mpl::bool_<boost::serialization::implementation_level<T>::value == boost::serialization::primitive_type>() );
	}

	template<class T>
	void load_override(const boost::serialization::nvp<T> & t, int) {
		T& x(t.value());
		*this >> x;
	}

	template<class T>
	void load_override(const boost::serialization::array<T> &t, int) {
		boost::serialization::array<T> tmp(t.address(), t.count());
		*this >> tmp;
	}

	template<class T>
	yarp_bottle_iarchive &operator>>(T &t) {
		this->load_override(t, 0);
		return *this;
	}

	yarp_bottle_iarchive &operator>>(int &t) {
		if (!m_bottle.get(0).isInt())
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		t = m_bottle.get(0).asInt();
		m_bottle = m_bottle.tail();
		return *this;
	}
	
	yarp_bottle_iarchive &operator>>(unsigned int &t) {
		if (!m_bottle.get(0).isInt())
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		t = m_bottle.get(0).asInt();
		m_bottle = m_bottle.tail();
		return *this;
	}
	
	yarp_bottle_iarchive &operator>>(long &t) {
		if (!m_bottle.get(0).isInt())
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		t = m_bottle.get(0).asInt();
		m_bottle = m_bottle.tail();
		return *this;
	}
	
	yarp_bottle_iarchive &operator>>(bool &t) {
		if (!m_bottle.get(0).isInt())
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		t = (m_bottle.get(0).asInt() > 0);
		m_bottle = m_bottle.tail();
		return *this;
	}
	
	yarp_bottle_iarchive &operator>>(double &t) {
		if (!m_bottle.get(0).isDouble())
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		t = m_bottle.get(0).asDouble();
		m_bottle = m_bottle.tail();
		return *this;
	}
	
	yarp_bottle_iarchive &operator>>(std::string &t) {
		if (!m_bottle.get(0).isString())
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		t = m_bottle.get(0).asString();
		m_bottle = m_bottle.tail();
		return *this;
	}
	
	yarp_bottle_iarchive &operator>>(char &t) {
		if (m_bottle.get(0).isInt())
			t = m_bottle.get(0).asInt();
		else if (m_bottle.get(0).isString())
			t = m_bottle.get(0).asString()[0];
		else
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		m_bottle = m_bottle.tail();
		return *this;
	}
	
	yarp_bottle_iarchive &operator>>(float &t) {
		if (!m_bottle.get(0).isDouble())
			throw barch::archive_exception(
					barch::archive_exception::unregistered_cast);
		t = m_bottle.get(0).asDouble();
		m_bottle = m_bottle.tail();
		return *this;
	}

	template<class T>
	yarp_bottle_iarchive& operator>>(std::vector<T>& t) {
        // Two ways of receiving a vector: as a sublist, or as bare T elements
        yarp::os::Bottle* b;
        if (m_bottle.get(0).isList())
            b = m_bottle.get(0).asList();
        else
            b = &m_bottle;
		yarp_bottle_iarchive v(*b);
        t.resize(b->size());
		for (typename std::vector<T>::iterator i = t.begin(); i != t.end(); i++)
			v >> (*i);
		m_bottle = m_bottle.tail();
		return *this;
	}

	template<class T>
	yarp_bottle_iarchive &operator&(T &t) {
		return this->operator>>(t);
	}

	template<class T>
	yarp_bottle_iarchive &load_a_type(T &t, boost::mpl::true_) {
		load_binary(&t, sizeof(T));
		return *this;
	}

	template<class T>
	yarp_bottle_iarchive &load_a_type(T &t, boost::mpl::false_) {
#if BOOST_VERSION >= 104100
		barch::detail::load_non_pointer_type<yarp_bottle_iarchive>::load_only::invoke(*this,t);
#else
		barch::detail::load_non_pointer_type<yarp_bottle_iarchive, T>::load_only::invoke(
				*this, t);
#endif
		return *this;
	}

	template<class ValueType>
	void load_array(boost::serialization::array<ValueType>& a, unsigned int) {
		load_binary(a.address(), a.count() * sizeof(ValueType));
	}

	int getArchiveSize() {
		return 0;
	}
};

class yarp_bottle_oarchive {
private:
	yarp::os::Bottle& m_bottle;

public:
	typedef boost::mpl::bool_<true> is_saving;
	typedef boost::mpl::bool_<false> is_loading;

	yarp_bottle_oarchive(yarp::os::Bottle& b) :
		m_bottle(b) {
	}

	unsigned int get_library_version() {
		return 0;
	}

	template<class T>
	const barch::detail::basic_pointer_iserializer* register_type() {
	}

	template<class T>
	yarp_bottle_oarchive& operator<<(const T& t) {
		return save_a_type(t, boost::mpl::bool_<
				boost::serialization::implementation_level<T>::value
						== boost::serialization::primitive_type>());
	}
	;

	yarp_bottle_oarchive& operator<<(const int& t) {
		m_bottle.addInt(t);
		return *this;
	}
	yarp_bottle_oarchive& operator<<(const unsigned int& t) {
		m_bottle.addInt(t);
		return *this;
	}
	yarp_bottle_oarchive& operator<<(const long& t) {
		m_bottle.addInt(t);
		return *this;
	}
	yarp_bottle_oarchive& operator<<(const bool& t) {
		m_bottle.addInt((t ? 1 : 0));
		return *this;
	}

	yarp_bottle_oarchive& operator<<(const double& t) {
		m_bottle.addDouble(t);
		return *this;
	}

	yarp_bottle_oarchive& operator<<(const std::string& t) {
		m_bottle.addString(t.c_str());
		return *this;
	}

	yarp_bottle_oarchive& operator<<(const char& t) {
		m_bottle.addInt(t);
		return *this;
	}

	yarp_bottle_oarchive& operator<<(const float& t) {
		m_bottle.addDouble(t);
		return *this;
	}

	template<class T>
	yarp_bottle_oarchive& operator<<(const std::vector<T>& t) {
		yarp_bottle_oarchive v(m_bottle.addList());
		for (typename std::vector<T>::const_iterator i = t.begin(); i
				!= t.end(); i++)
			v << *i;
		return *this;
	}

	template<class T>
	yarp_bottle_oarchive& operator&(const T& t) {
		return *this << t;
	}
	;

	void save_binary(const void* address, std::size_t count) {
		assert(false);
	}
	;

	void save_object(const void *x,
			const barch::detail::basic_oserializer & bos) {
		assert(false);
	}

	template<class T>
	yarp_bottle_oarchive &save_a_type(T const &t, boost::mpl::true_) {
		save_binary(&t, sizeof(T));
		return *this;
	}

	template<class T>
	yarp_bottle_oarchive &save_a_type(T const &t, boost::mpl::false_) {
#if BOOST_VERSION >= 104100
		barch::detail::save_non_pointer_type<yarp_bottle_oarchive>::save_only::invoke(*this,t);
#else
		barch::detail::save_non_pointer_type<yarp_bottle_oarchive, T>::save_only::invoke(
				*this, t);
#endif
		return *this;
	}

	template<class ValueType>
	void save_array(boost::serialization::array<ValueType> const& a,
			unsigned int) {
		assert(false);
	}

};

#endif
