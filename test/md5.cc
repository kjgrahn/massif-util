/**
 * $Id: test_md5.cc,v 1.6 2011-02-06 07:37:55 grahn Exp $
 *
 * Copyright (c) 2010 J?rgen Grahn
 * All rights reserved.
 *
 */
#include <md5.h>
#include <md5pp.h>

#include <orchis.h>
#include <string>
#include <vector>
#include <list>
#include <sstream>

namespace {

    void assert_eq(const std::string& ref,
		   const md5::Digest& digest)
    {
	orchis::assert_eq(ref, digest.hex());
    }

    void assert_eq(const std::string& ref,
		   const unsigned char* digest)
    {
	const md5::Digest d(digest);
	assert_eq(ref, d);
    }
}

namespace md5 {

    using orchis::TC;

    namespace c {

	void test(TC)
	{
	    MD5_CTX ctx;
	    MD5_Init(&ctx);
	    {
		unsigned char digest[16];
		MD5_CTX c = ctx;
		MD5_Final(digest, &c);
		assert_eq("d41d8cd98f00b204e9800998ecf8427e",
			  digest);
	    }

	    MD5_Update(&ctx, "Hello, world!", 13);
	    {
		unsigned char digest[16];
		MD5_CTX c = ctx;
		MD5_Final(digest, &c);
		assert_eq("6cd3556deb0da54bca060b4c39479839",
			  digest);
	    }

	    MD5_Update(&ctx,
		       "xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxx", 53);
	    {
		unsigned char digest[16];
		MD5_CTX c = ctx;
		MD5_Final(digest, &c);
		assert_eq("69bb639b030e67e4c360d0206abca653",
			  digest);
	    }
	}
    }

    namespace cpp {

	const char hw[] = "Hello, world!";

	void test(TC)
	{
	    md5::Ctx ctx;
	    assert_eq("d41d8cd98f00b204e9800998ecf8427e",
		      ctx.digest());

	    ctx.update(hw, 13);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());

	    ctx.update("xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxx", 53);
	    assert_eq("69bb639b030e67e4c360d0206abca653",
		      ctx.digest());
	}

	void string(TC)
	{
	    const std::string d(hw);
	    md5::Ctx ctx;
	    ctx.update(d);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	}

	void vector(TC)
	{
	    const std::vector<char> d(hw, hw+13);
	    md5::Ctx ctx;
	    ctx.update(d);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	}

	void iter(TC)
	{
	    const std::list<unsigned char> d(hw, hw+13);
	    md5::Ctx ctx;
	    ctx.update(d.begin(), d.end());
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	}

	void istream(TC)
	{
	    std::istringstream is(hw);
	    md5::Ctx ctx;
	    ctx.update(is);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	    orchis::assert_(is.eof());
	}

	void digestobj(TC)
	{
	    md5::Ctx ctx;
	    ctx.update(hw);
	    const md5::Digest digest = ctx.digest();

	    std::ostringstream o1;
	    o1 << "foo " << digest.hex();
	    orchis::assert_eq("foo 6cd3556deb0da54bca060b4c39479839",
			      o1.str());

	    std::ostringstream o2;
	    o2 << "bar " << digest;
	    orchis::assert_eq("bar 6cd3556deb0da54bca060b4c39479839",
			      o2.str());
	}
    }
}
