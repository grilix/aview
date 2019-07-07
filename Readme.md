# AView

Experimenting on a ruby extension for strings concatenation.

## Buffer

A `Buffer` contains the string chunks to be concatenated.

Example usage:

```ruby
buffer = AView::Buffer.new('some string')
buffer.append('some other string')

buffer.to_s # => "some stringsome other string"

buffer2 = AView::Buffer.new('[')
buffer2.append_buffer(buffer)
buffer2.append(']')

buffer2.to_s # => "[some stringsome other string]"

```

## Motivation?

Internally, each chunk is stored on its own place and appending strings don't require the
whole object to be rellocated, so it should give room for some performance improvements,
but mostly some nice features, like undoing or conditional appending using dynamic
expressions (like blocks). But these are just some ideas that will probably never
implement.

For now, I'm leaving this around. I might come back to it later. Or not.
