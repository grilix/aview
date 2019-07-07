Gem::Specification.new do |s|
  s.name        = 'aview'
  s.version     = '0.0.1'
  s.date        = '2019-07-07'
  s.summary     = "A string library"
  s.description = "Create some strings with extensions"
  s.authors     = ["Gonzalo Arreche"]
  s.email       = 'gonzaloarreche@gmail.com'
  s.files       = [
    "lib/aview.rb",
  ]
  s.homepage    = 'https://github.com/grilix/aview'
  s.license       = 'MIT'
  s.extensions = ['ext/extconf.rb']
  s.require_paths = ['lib', 'ext']

  s.add_development_dependency "rspec"
end
