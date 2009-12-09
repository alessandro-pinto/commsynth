/*
 Copyright (c) 2007-2008 The Regents of the University of California.
 All rights reserved.

 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the
 above copyright notice and the following two paragraphs appear in all
 copies of this software and that appropriate acknowledgments are made
 to the research of the COSI group.

 IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
 PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
 CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

 Author : Alessandro Pinto <apinto@eecs.berkeley.edu>
 University of California, Berkeley
 545 Cory Hall, Berkeley, CA 94720
 */
#ifndef COSISYSCUTIL_H_
#define COSISYSCUTIL_H_

stringstream _cosiptpname_ ;

vector< DestIp<FLITWIDTH>* > _CosiDestIpVector_ ;


#define COSI_SOURCE_TO_IF_CONNECTION( s , d , inp , layer , length ) cout << "Connecting source " << s << " to interface " << d << endl ; \
    _cosiptpname_ << "S" << #s << "_Sif" << #d << "_in" << #inp ; \
    PtP< FLITWIDTH > S##s##_Sif##d##_in##inp( _cosiptpname_.str( ).c_str( ) ,  TechParam , layer , length ) ; \
    sc_signal< sc_bv< FLITWIDTH > > S##s##_Data ;				\
    sc_signal< bool > S##s##_Valid ;					\
    sc_signal< bool > S##s##_Ack ;					\
    sc_signal< bool > S##s##_Full ;					\
    S##s.DataOut( S##s##_Data ) ;						\
    S##s.ValidOut( S##s##_Valid ) ;					\
    S##s.AckIn( S##s##_Ack ) ;						\
    S##s.FullIn( S##s##_Full ) ;						\
    S##s##_Sif##d##_in##inp.DataIn( S##s##_Data ) ;				\
    S##s##_Sif##d##_in##inp.ValidIn( S##s##_Valid ) ;				\
    S##s##_Sif##d##_in##inp.AckOut( S##s##_Ack ) ;				\
    S##s##_Sif##d##_in##inp.FullOut( S##s##_Full ) ;				\
    sc_signal< sc_bv< FLITWIDTH > > Sif##d##_in##inp##_Data ;			\
    sc_signal< bool > Sif##d##_in##inp##_Valid ;				\
    sc_signal< bool > Sif##d##_in##inp##_Ack ;				\
    sc_signal< bool > Sif##d##_in##inp##_Full ;				\
    Sif##d.DataIn[ inp ]( Sif##d##_in##inp##_Data ) ;				\
    Sif##d.ValidIn[ inp ]( Sif##d##_in##inp##_Valid ) ;				\
    Sif##d.AckOut[ inp ]( Sif##d##_in##inp##_Ack ) ;				\
    Sif##d.FullOut[ inp ]( Sif##d##_in##inp##_Full ) ;				\
    S##s##_Sif##d##_in##inp.DataOut( Sif##d##_in##inp##_Data ) ;			\
    S##s##_Sif##d##_in##inp.ValidOut( Sif##d##_in##inp##_Valid ) ;			\
    S##s##_Sif##d##_in##inp.AckIn( Sif##d##_in##inp##_Ack ) ;			\
    S##s##_Sif##d##_in##inp.FullIn( Sif##d##_in##inp##_Full ) ;

#define COSI_ROUTER_TO_ROUTER_CONNECTION( s , outp , d , inp , layer , length ) cout << "Connecting router " << s << " to router " << d << endl ; \
    _cosiptpname_ << "R" << #s << "_out" << #outp << "_R" << #d << "_in" << #inp ; \
    PtP< FLITWIDTH > R##s##_out##outp##_R##d##_in##inp( _cosiptpname_.str().c_str( ) ,  TechParam , layer , length ) ; \
    sc_signal< sc_bv< FLITWIDTH > > R##s##_out##outp##_Data ;		\
    sc_signal< bool > R##s##_out##outp##_Valid ;			\
    sc_signal< bool > R##s##_out##outp##_Ack ;				\
    sc_signal< bool > R##s##_out##outp##_Full ;				\
    R##s.DataOut[ outp ]( R##s##_out##outp##_Data ) ;			\
    R##s.ValidOut[ outp ]( R##s##_out##outp##_Valid ) ;			\
    R##s.AckIn[ outp ]( R##s##_out##outp##_Ack ) ;			\
    R##s.FullIn[ outp ]( R##s##_out##outp##_Full ) ;			\
    R##s##_out##outp##_R##d##_in##inp.DataIn( R##s##_out##outp##_Data ) ; \
    R##s##_out##outp##_R##d##_in##inp.ValidIn( R##s##_out##outp##_Valid ) ; \
    R##s##_out##outp##_R##d##_in##inp.AckOut( R##s##_out##outp##_Ack ) ; \
    R##s##_out##outp##_R##d##_in##inp.FullOut( R##s##_out##outp##_Full ) ; \
    sc_signal< sc_bv< FLITWIDTH > > R##d##_in##inp##_Data ;		\
    sc_signal< bool > R##d##_in##inp##_Valid ;				\
    sc_signal< bool > R##d##_in##inp##_Ack ;				\
    sc_signal< bool > R##d##_in##inp##_Full ;				\
    R##d.DataIn[ inp ]( R##d##_in##inp##_Data ) ;			\
    R##d.ValidIn[ inp ]( R##d##_in##inp##_Valid ) ;			\
    R##d.AckOut[ inp ]( R##d##_in##inp##_Ack ) ;			\
    R##d.FullOut[ inp ]( R##d##_in##inp##_Full ) ;			\
    R##s##_out##outp##_R##d##_in##inp.DataOut( R##d##_in##inp##_Data ) ; \
    R##s##_out##outp##_R##d##_in##inp.ValidOut( R##d##_in##inp##_Valid ) ; \
    R##s##_out##outp##_R##d##_in##inp.AckIn( R##d##_in##inp##_Ack ) ;	\
    R##s##_out##outp##_R##d##_in##inp.FullIn( R##d##_in##inp##_Full ) ;

#define COSI_IF_TO_ROUTER_CONNECTION( s , outp , d , inp , layer , length ) cout << "Connecting interface " << s << " to router " << d << endl ; \
    _cosiptpname_ << "Sif" << #s << "_out" << #outp << "_R" << #d << "_in" << #inp ; \
    PtP< FLITWIDTH > Sif##s##_out##outp##_R##d##_in##inp( _cosiptpname_.str().c_str( ) ,  TechParam , layer , length ) ; \
    sc_signal< sc_bv< FLITWIDTH > > Sif##s##_out##outp##_Data ;		\
    sc_signal< bool > Sif##s##_out##outp##_Valid ;			\
    sc_signal< bool > Sif##s##_out##outp##_Ack ;				\
    sc_signal< bool > Sif##s##_out##outp##_Full ;				\
    Sif##s.DataOut[ outp ]( Sif##s##_out##outp##_Data ) ;			\
    Sif##s.ValidOut[ outp ]( Sif##s##_out##outp##_Valid ) ;			\
    Sif##s.AckIn[ outp ]( Sif##s##_out##outp##_Ack ) ;			\
    Sif##s.FullIn[ outp ]( Sif##s##_out##outp##_Full ) ;			\
    Sif##s##_out##outp##_R##d##_in##inp.DataIn( Sif##s##_out##outp##_Data ) ; \
    Sif##s##_out##outp##_R##d##_in##inp.ValidIn( Sif##s##_out##outp##_Valid ) ; \
    Sif##s##_out##outp##_R##d##_in##inp.AckOut( Sif##s##_out##outp##_Ack ) ; \
    Sif##s##_out##outp##_R##d##_in##inp.FullOut( Sif##s##_out##outp##_Full ) ; \
    sc_signal< sc_bv< FLITWIDTH > > R##d##_in##inp##_Data ;		\
    sc_signal< bool > R##d##_in##inp##_Valid ;				\
    sc_signal< bool > R##d##_in##inp##_Ack ;				\
    sc_signal< bool > R##d##_in##inp##_Full ;				\
    R##d.DataIn[ inp ]( R##d##_in##inp##_Data ) ;			\
    R##d.ValidIn[ inp ]( R##d##_in##inp##_Valid ) ;			\
    R##d.AckOut[ inp ]( R##d##_in##inp##_Ack ) ;			\
    R##d.FullOut[ inp ]( R##d##_in##inp##_Full ) ;			\
    Sif##s##_out##outp##_R##d##_in##inp.DataOut( R##d##_in##inp##_Data ) ; \
    Sif##s##_out##outp##_R##d##_in##inp.ValidOut( R##d##_in##inp##_Valid ) ; \
    Sif##s##_out##outp##_R##d##_in##inp.AckIn( R##d##_in##inp##_Ack ) ;	\
    Sif##s##_out##outp##_R##d##_in##inp.FullIn( R##d##_in##inp##_Full ) ;

#define COSI_ROUTER_TO_IF_CONNECTION( s , outp , d , inp , layer , length ) cout << "Connecting router " << s << " to interface " << d << endl ; \
    _cosiptpname_ << "R" << #s << "_out" << #outp << "_Dif" << #d << "_in" << #inp ; \
    PtP< FLITWIDTH > R##s##_out##outp##_Dif##d##_in##inp( _cosiptpname_.str().c_str( ) ,  TechParam , layer , length ) ; \
    sc_signal< sc_bv< FLITWIDTH > > R##s##_out##outp##_Data ;		\
    sc_signal< bool > R##s##_out##outp##_Valid ;			\
    sc_signal< bool > R##s##_out##outp##_Ack ;				\
    sc_signal< bool > R##s##_out##outp##_Full ;				\
    R##s.DataOut[ outp ]( R##s##_out##outp##_Data ) ;			\
    R##s.ValidOut[ outp ]( R##s##_out##outp##_Valid ) ;			\
    R##s.AckIn[ outp ]( R##s##_out##outp##_Ack ) ;			\
    R##s.FullIn[ outp ]( R##s##_out##outp##_Full ) ;			\
    R##s##_out##outp##_Dif##d##_in##inp.DataIn( R##s##_out##outp##_Data ) ; \
    R##s##_out##outp##_Dif##d##_in##inp.ValidIn( R##s##_out##outp##_Valid ) ; \
    R##s##_out##outp##_Dif##d##_in##inp.AckOut( R##s##_out##outp##_Ack ) ; \
    R##s##_out##outp##_Dif##d##_in##inp.FullOut( R##s##_out##outp##_Full ) ; \
    sc_signal< sc_bv< FLITWIDTH > > Dif##d##_in##inp##_Data ;		\
    sc_signal< bool > Dif##d##_in##inp##_Valid ;				\
    sc_signal< bool > Dif##d##_in##inp##_Ack ;				\
    sc_signal< bool > Dif##d##_in##inp##_Full ;				\
    Dif##d.DataIn[ inp ]( Dif##d##_in##inp##_Data ) ;			\
    Dif##d.ValidIn[ inp ]( Dif##d##_in##inp##_Valid ) ;			\
    Dif##d.AckOut[ inp ]( Dif##d##_in##inp##_Ack ) ;			\
    Dif##d.FullOut[ inp ]( Dif##d##_in##inp##_Full ) ;			\
    R##s##_out##outp##_Dif##d##_in##inp.DataOut( Dif##d##_in##inp##_Data ) ; \
    R##s##_out##outp##_Dif##d##_in##inp.ValidOut( Dif##d##_in##inp##_Valid ) ; \
    R##s##_out##outp##_Dif##d##_in##inp.AckIn( Dif##d##_in##inp##_Ack ) ;	\
    R##s##_out##outp##_Dif##d##_in##inp.FullIn( Dif##d##_in##inp##_Full ) ;


#define COSI_IF_TO_DEST_CONNECTION( s , outp , d  , layer , length ) cout << "Connecting interface " << s << " to destination " << d << endl ; \
    _cosiptpname_ << "Dif" << #s <<  "_out" << #outp << "_D"  << #d; \
    PtP< FLITWIDTH > Dif##s##_out##outp##_D##d( _cosiptpname_.str().c_str( )  ,  TechParam , layer , length ) ; \
    sc_signal< sc_bv< FLITWIDTH > > Dif##s##_out##outp##_Data ;		\
    sc_signal< bool > Dif##s##_out##outp##_Valid ;				\
    sc_signal< bool > Dif##s##_out##outp##_Ack ;				\
    sc_signal< bool > Dif##s##_out##outp##_Full ;				\
    Dif##s.DataOut[ outp ]( Dif##s##_out##outp##_Data ) ;			\
    Dif##s.ValidOut[ outp ]( Dif##s##_out##outp##_Valid ) ;			\
    Dif##s.AckIn[ outp ]( Dif##s##_out##outp##_Ack ) ;			\
    Dif##s.FullIn[ outp ]( Dif##s##_out##outp##_Full ) ;			\
    Dif##s##_out##outp##_D##d.DataIn( Dif##s##_out##outp##_Data ) ;		\
    Dif##s##_out##outp##_D##d.ValidIn( Dif##s##_out##outp##_Valid ) ;	\
    Dif##s##_out##outp##_D##d.AckOut( Dif##s##_out##outp##_Ack ) ;		\
    Dif##s##_out##outp##_D##d.FullOut( Dif##s##_out##outp##_Full ) ;	\
    sc_signal< sc_bv< FLITWIDTH > > D##d##_Data ;			\
    sc_signal< bool > D##d##_Valid ;					\
    sc_signal< bool > D##d##_Ack ;					\
    sc_signal< bool > D##d##_Full ;					\
    D##d.DataIn( D##d##_Data ) ;					\
    D##d.ValidIn( D##d##_Valid ) ;					\
    D##d.AckOut( D##d##_Ack ) ;						\
    D##d.FullOut( D##d##_Full ) ;					\
    Dif##s##_out##outp##_D##d.DataOut( D##d##_Data ) ;			\
    Dif##s##_out##outp##_D##d.ValidOut( D##d##_Valid ) ;			\
    Dif##s##_out##outp##_D##d.AckIn( D##d##_Ack ) ;			\
    Dif##s##_out##outp##_D##d.FullIn( D##d##_Full ) ;


void _CosiAddDestIp_( DestIp<FLITWIDTH>* D ) {
  _CosiDestIpVector_.push_back( D ) ;
}

#endif
