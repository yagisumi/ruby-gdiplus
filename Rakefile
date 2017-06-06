require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList['test/**/*_test.rb']
end

Rake::Task[:test].prerequisites << :clean
Rake::Task[:test].prerequisites << :compile

Rake::TestTask.new(:testonly) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList['test/**/*_test.rb']
end

require "yard"
YARD::Rake::YardocTask.new(:doc) do |t|
  require_relative "lib/gdiplus/yard_gdiplus_handler"
  t.options.push(*%w[--no-private -p templates -t gdiplus --tag figure:Figure])
  t.files = FileList['lib/**/*.rb', 'ext/**/*.cpp']
end

require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("gdiplus") do |ext|
  ext.lib_dir = "lib/gdiplus"
end

task :default => [:clobber, :compile, :test]
