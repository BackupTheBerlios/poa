class PoaException
{
public:
    PoaException( const QString& msg="")
        : msg_( msg )
    { }

    QString message() const {return msg_;};

private:
    QString msg_;
};
