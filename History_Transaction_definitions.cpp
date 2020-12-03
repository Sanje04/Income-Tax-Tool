#ifndef _HISTORY_TRANSACTION_CPP_
#define _HISTORY_TRANSACTION_CPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//
// Constructor
// TASK 1
//
 Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
                            unsigned int month_date,  unsigned year_date,
                            bool buy_sell_trans,  unsigned int number_shares,
                            double trans_amount )
 {
    symbol = ticker_symbol;
    day = day_date;
    month = month_date;
    year = year_date;
    trans_type = ( buy_sell_trans )?"Buy":"Sell";
    shares = number_shares;
    amount = trans_amount;
    trans_id = assigned_trans_id++;
    p_next = nullptr;
    acb= 0.0;
    acb_per_share=0.0;
    share_balance=0;
    cgl=0.0;
 }
// Destructor
// TASK 1
//
Transaction::~Transaction()
{
   symbol = "";
   trans_type = "";
   day = 0;
   month = 0;
   year = 0;
   shares=0;
   amount =0.0;
   p_next=nullptr;
   trans_id = 0;
   acb= 0.0;
   acb_per_share=0.0;
   share_balance=0;
   cgl=0.0;
 }

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<( Transaction const &other )
{
    if (year < other.year)
    {
	 	 return true;
    }
    else if (year == other.year)
    {
        if (month < other.month)
        {
            return true;
        }
        else if (month == other.month)
        {
            if (day < other.day) 
            {
                return true;
            }
            else 
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    } 
    else 
    {
        return false;
    }
}


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History() {
  p_head = nullptr;
}

// Destructor
// TASK 3
//
History::~History(){
	while(p_head!=nullptr){
		Transaction *head = p_head;
		p_head = p_head ->get_next();
		delete head;
		head = nullptr;
	}
}

// read_transaction(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
	ece150::open_file();

	while (ece150::next_trans_entry()){
		Transaction *t = new Transaction(
			ece150::get_trans_symbol(), ece150::get_trans_day(),
			ece150::get_trans_month(), ece150::get_trans_year(),
			ece150::get_trans_type(), ece150::get_trans_shares(),
			ece150::get_trans_amount());

		History::insert(t);
	}
	ece150::close_file();

}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans){
	if (p_head == nullptr){
        //no head.. add new head
		p_head = p_new_trans;
	}
	else
    {
		Transaction *tl = p_head;
        //look for tail
		while (tl->get_next() != nullptr){
			tl = tl->get_next();
		}
        tl->set_next(p_new_trans);
	}
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){
	Transaction *p_sorted;
	Transaction *p_insert;
	Transaction *p_temp;
	if (p_head==nullptr || p_head->get_next()==nullptr){

		return;
	}
	p_sorted = p_head;
	p_head = p_head->get_next();
	p_sorted->set_next(nullptr);
	while (!(p_head == nullptr)){
	    p_temp = p_head;
	    p_head = p_head->get_next();
	    p_temp->set_next(nullptr);
	    if (*p_temp < *p_sorted){
		    p_temp -> set_next(p_sorted);
		    p_sorted = p_temp;
	    }
	    else{
		    p_insert = p_sorted;
		    while ((p_insert->get_next()!= nullptr) &&  !(*p_temp<*(p_insert->get_next()))){
			    p_insert = p_insert->get_next();
		    }
			p_temp->set_next(p_insert->get_next());
		    p_insert->set_next(p_temp);
	    }
	}
	p_head=p_sorted;

}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){
    double share_balance = 0.00;
    double acb = 0.00;
    double acb_per_share = 0.00;
    double cgl = 0.00;

	Transaction *t = p_head;
	while (t != nullptr){

		if (t->get_trans_type()){
            // Buy
            share_balance += t->get_shares();			
            t->set_share_balance(share_balance);            

            acb += t->get_amount();
			t->set_acb(acb);

			acb_per_share = (acb/share_balance);
            t->set_acb_per_share(acb_per_share);

		}
		else
        {
            // Sell
            cgl = t->get_amount() - (t->get_shares() * (acb/share_balance));
			acb = acb - (t->get_shares() * (acb/share_balance));
			share_balance -= t->get_shares();
			
            t->set_share_balance(share_balance);            ;
			t->set_acb(acb);
            t->set_acb_per_share(acb_per_share);
            t->set_cgl(cgl);
		}
		t = t->get_next();
	}
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year)
{
    double cgl = 0.00;
	Transaction *t = p_head;
	while (t != nullptr){
		if (t->get_year() == year){
			cgl = cgl + t->get_cgl();
		}
		t = t->get_next();
	}
	return cgl;
}

// print() Print the transaction history.
//TASK 9
//
void History::print(){
	Transaction *t = p_head;

    std::cout << "  =========================== BEGIN TRANSACTION HISTORY =============================" <<std::endl;
    std::cout << "  ID SYMB  Day  Mth Year  Type  Shrs     Amount        ACB  Bal  ACB/Share        CGL" << std::endl;
    std::cout << "  -----------------------------------------------------------------------------------" << std::endl;

	while (t != nullptr){
	 std::cout << std::fixed << std::setprecision(2);
	  std::cout << std::setw(4) << t->get_trans_id() << " "
	    << std::setw(4) << t->get_symbol() << " "
	    << std::setw(4) << t->get_day() << " "
	    << std::setw(4) << t->get_month() << " "
	    << std::setw(4) << t->get_year() << " ";

	  if ( t->get_trans_type() ) {
	    std::cout << "  Buy  ";
	  } else { std::cout << "  Sell "; }

	  std::cout << std::setw(4) << t->get_shares() << " "
	    << std::setw(10) << t->get_amount() << " "
	    << std::setw(10) << t->get_acb() << " " << std::setw(4) << t->get_share_balance() << " "
	    << std::setw(10) << std::setprecision(3) << t->get_acb_per_share() << " "
	    << std::setw(10) << std::setprecision(3) << t->get_cgl()
	    << std::endl;
	  t = t -> get_next();
	}
    std::cout << "  ============================ END TRANSACTION HISTORY ==============================" <<std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }

#endif
