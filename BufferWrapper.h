#ifndef BUFFERWRAPPER_H
#define BUFFERWRAPPER_H

#include <CL/cl2.hpp>

template< typename WRAPPED_TYPE, int MEM_FLAGS = CL_MEM_READ_WRITE, typename CONTEXT_TYPE = cl::Context >
class BufferWrapper {
public:
    using WrappedType = WRAPPED_TYPE;

    BufferWrapper( WRAPPED_TYPE const &wrappedType, CONTEXT_TYPE const& context )
        : m_allocatedBuffer( context, MEM_FLAGS, sizeof( WRAPPED_TYPE ) ) {}

    BufferWrapper( BufferWrapper&& rhs ) : m_allocatedBuffer( std::move(rhs.m_allocatedBuffer) ) {};
    operator cl::Buffer&() { return m_allocatedBuffer; }
    std::size_t bufferSize() { return sizeof( WRAPPED_TYPE ); }


private:
    cl::Buffer m_allocatedBuffer;
};

template< typename T, typename CONTEXT_TYPE, int MEM_FLAGS = CL_MEM_READ_WRITE >
BufferWrapper< T, MEM_FLAGS, CONTEXT_TYPE > MakeBufferWrapper( T const& t, CONTEXT_TYPE context ) {
    return BufferWrapper< T, MEM_FLAGS, CONTEXT_TYPE >( t, context );
}


#endif
