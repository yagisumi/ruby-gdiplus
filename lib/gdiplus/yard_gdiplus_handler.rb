# frozen_string_literal: true

# @private
class GdiplusEnumHandler < YARD::Handlers::C::Base
  MATCH_ENUM_1 = /(?:define_enumint|define_enumflags)\((\w+), \w+, "(\w+)", (-?\w+)\)/
  MATCH_ENUM_2 = /gdip_enum_define<[^>]+>\((\w+), \w+, "(\w+)"/
  
  handles MATCH_ENUM_1
  handles MATCH_ENUM_2
  statement_class BodyStatement

  process do
    statement.source.scan(MATCH_ENUM_1) do |klass, const_name, value|
      handle_constants("const", klass, const_name, value)
      handle_enum_method(klass, const_name)
    end
    statement.source.scan(MATCH_ENUM_2) do |var_name, const_name|
      klass = var_name.sub(/^c/, '')
      value = "#{klass}#{const_name}"
      handle_constants("const", var_name, const_name, value)
      handle_enum_method(var_name, const_name)
    end
  end
  
  def handle_enum_method(var_name, name)
    namespace = namespace_for_variable(var_name)
    register MethodObject.new(namespace, name, :class) do |obj|
      register_visibility(obj, :public)
      
      klass = var_name.sub(/^c/, '')
      obj.add_tag(YARD::Tags::Tag.new(:return, "{Gdiplus::#{klass}::#{name}}", klass))
      obj.explicit = true
    end
  end
end

# @private
class GdiplusClassMethodAliasHandler < YARD::Handlers::C::Base
  MATCH2 = /rb_define_alias
             \s*\(\s*rb_singleton_class\s*\(\s*([\w\.]+)\s*\)\s*,
             \s*"([^"]+)",
             \s*"([^"]+)"\s*\)/xm
  handles MATCH2
  statement_class BodyStatement

  process do
    statement.source.scan(MATCH2) do |var_name, new_name, old_name|
      var_name = "rb_cObject" if var_name == "rb_mKernel"
      handle_alias(var_name, new_name, old_name, :class)
    end
  end
end

# @private
class GdiplusAttrHandler < YARD::Handlers::C::Base
  MATCH_ATTR_R4 = /(CLASS_)?ATTR_R\((\w+), (\w+), (\w+), (\w+)\)/
  MATCH_ATTR_R5 = /(CLASS_)?ATTR_R\((\w+), (\w+), (\w+), (\w+), (\w+)\)/
  MATCH_ATTR_R_Q = /(CLASS_)?ATTR_R_Q\((\w+), (\w+), (\w+), (\w+)\)/
  MATCH_ATTR_RW = /(CLASS_)?ATTR_RW\((\w+), (\w+), (\w+), (\w+)\)/
 
  handles MATCH_ATTR_R4
  handles MATCH_ATTR_R5
  handles MATCH_ATTR_R_Q
  handles MATCH_ATTR_RW
  statement_class BodyStatement

  process do
    statement.source.scan(MATCH_ATTR_R4) do |class_scope, klass, name1, name2, prefix|
      if class_scope
        handle_method(:class, klass, name1, "gdip_#{prefix}_s_get_#{name2}")
        handle_attribute(klass, name1, 1, 0, :class)
        handle_alias(klass, name2, name1, :class)
      else
        handle_method(:instance, klass, name1, "gdip_#{prefix}_get_#{name2}")
        handle_attribute(klass, name1, 1, 0)
        handle_alias(klass, name2, name1)
      end
      
    end
    statement.source.scan(MATCH_ATTR_R5) do |class_scope, klass, name1, name2, prefix, name3|
      if class_scope
        handle_method(:class, klass, name1, "gdip_#{prefix}_s_get_#{name3}")
        handle_attribute(klass, name1, 1, 0, :class)
        handle_alias(klass, name2, name1, :class)
      else
        handle_method(:instance, klass, name1, "gdip_#{prefix}_get_#{name3}")
        handle_attribute(klass, name1, 1, 0)
        handle_alias(klass, name2, name1)
      end
    end
    statement.source.scan(MATCH_ATTR_R_Q) do |class_scope, klass, name1, name2, prefix|
      if class_scope
        handle_method(:instance, klass, name1, "gdip_#{prefix}_s_get_#{name2}")
        handle_attribute(klass, name1, 1, 0, :class)
        handle_alias(klass, "#{name2}?", name1, :class)
      else
        handle_method(:instance, klass, name1, "gdip_#{prefix}_get_#{name2}")
        handle_attribute(klass, name1, 1, 0)
        handle_alias(klass, "#{name2}?", name1)
      end
    end
    statement.source.scan(MATCH_ATTR_RW) do |class_scope, klass, name1, name2, prefix|
      if class_scope
        handle_method(:class, klass, name1, "gdip_#{prefix}_s_get_#{name2}")
        handle_attribute(klass, name1, 1, 1, :class)
        handle_alias(klass, name2, name1, :class)
      else
        handle_method(:instance, klass, name1, "gdip_#{prefix}_get_#{name2}")
        handle_attribute(klass, name1, 1, 1, :class)
        handle_alias(klass, name2, name1, :class)
      end
    end
    
  end
  
end


$superclass_path_map = {
  "cGpObject" => "Gdiplus::Internals::GpObject",
  "cEnumInt" => "Gdiplus::Internals::EnumInt",
  "cEnumFlags" => "Gdiplus::Internals::EnumFlags",
  "cImage" => "Gdiplus::Image",
  "cGuid" => "Gdiplus::Guid",
  "cBrush" => "Gdiplus::Brush",
}

# Copyright (c) 2007-2016 Loren Segal
# @private
module YARD
  module Handlers
    module C
      module HandlerMethods
        ### modified to use variable name to class path mapping ($superclass_path_map)
        def handle_class(var_name, class_name, parent, in_module = nil)
          parent = nil if parent == "0"
          namespace = in_module ? ensure_variable_defined!(in_module) : Registry.root
          if namespace.nil?
            raise Parser::UndocumentableError,
              "class #{class_name}. Cannot find definition for parent namespace."
          end

          register ClassObject.new(namespace, class_name) do |obj|
            if parent
              parent_class = namespace_for_variable(parent)
              if parent_class.is_a?(Proxy)
                if path = ($superclass_path_map || {})[parent]
                  obj.superclass = path
                else
                  obj.superclass = "::#{parent_class.path}"
                end
                obj.superclass.type = :class
              else
                obj.superclass = parent_class
              end
            end

            namespaces[var_name] = obj
            register_file_info(obj, statement.file, statement.line)
          end
        end
        
        ### modified the condition of @return insertion
        def handle_method(scope, var_name, name, func_name, _source_file = nil)
          visibility = :public
          case scope
          when Symbol ###; scope = scope
          when "singleton_method"; scope = :class
          when "module_function"; scope = :module
          when "private_method"; scope = :instance; visibility = :private
          else; scope = :instance
          end

          namespace = namespace_for_variable(var_name)

          # Is this method being defined on a core Ruby class or module?
          if namespace.is_a?(Proxy)
            if var_name =~ /^rb_c(\w+)/ && YARD::CodeObjects::BUILTIN_CLASSES.include?($1)
              namespace = namespaces[var_name] = YARD::CodeObjects::ClassObject.new(:root, $1)
            elsif var_name =~ /^rb_m(\w+)/ && YARD::CodeObjects::BUILTIN_MODULES.include?($1)
              namespace = namespaces[var_name] = YARD::CodeObjects::ModuleObject.new(:root, $1)
            end
          end

          return if namespace.nil? # XXX: raise UndocumentableError might be too noisy.
          register MethodObject.new(namespace, name, scope) do |obj|
            register_visibility(obj, visibility)
            find_method_body(obj, func_name)
            obj.explicit = true
            if name.end_with?("?") && !(obj.tag(:return) || (obj.tag(:overload) && obj.tag(:overload).tag(:return)))
              obj.add_tag(Tags::Tag.new(:return, '', 'Boolean'))
            end
          end
        end
        
        ### modified to accept a scope parameter
        def handle_attribute(var_name, name, read, write, scope = :instance)
          values = {:read => read.to_i, :write => write.to_i}
          {:read => name, :write => "#{name}="}.each do |type, meth_name|
            next unless values[type] > 0
            obj = handle_method(scope, var_name, meth_name, nil)
            register_file_info(obj, statement.file, statement.line)
            obj.namespace.attributes[scope][name] ||= SymbolHash[:read => nil, :write => nil]
            obj.namespace.attributes[scope][name][type] = obj
          end
        end
        
        ### modified to accept a scope parameter
        def handle_alias(var_name, new_name, old_name, scope = :instance)
          namespace = namespace_for_variable(var_name)
          return if namespace.nil?
          new_meth = new_name.to_sym
          old_meth = old_name.to_sym
          old_obj = namespace.child(:name => old_meth, :scope => scope)
          new_obj = register MethodObject.new(namespace, new_meth, scope) do |o|
            register_visibility(o, visibility)
            register_file_info(o, statement.file, statement.line)
          end

          if old_obj
            new_obj.signature = old_obj.signature
            new_obj.source = old_obj.source
            new_obj.docstring = old_obj.docstring
            new_obj.docstring.object = new_obj
          else
            new_obj.signature = "def #{new_meth}" # this is all we know.
          end

          namespace.aliases[new_obj] = old_meth
        end
      end
      
    end
  end
  
  # @private
  ### modified to link types with not method but class in tags
  module Templates::Helpers
    module HtmlHelper
      def format_types(typelist, brackets = true)
        return unless typelist.is_a?(Array)
        @in_format_types = true
        list = typelist.map do |type|
          type = type.gsub(/([<>])/) { h($1) }
          type = type.gsub(/([\w:]+)/) { $1 == "lt" || $1 == "gt" ? $1 : linkify($1, $1) }
          "<tt>" + type + "</tt>"
        end
        @in_format_types = false
        list.empty? ? "" : (brackets ? "(#{list.join(", ")})" : list.join(", "))
      end
      
      def link_object(obj, title = nil, anchor = nil, relative = true)
        return title if obj.nil?
        obj = Registry.resolve(object, obj, true, true, @in_format_types ? :class : nil) if obj.is_a?(String)
        if title
          title = title.to_s
        elsif object.is_a?(CodeObjects::Base)
          # Check if we're linking to a class method in the current
          # object. If we are, create a title in the format of
          # "CurrentClass.method_name"
          if obj.is_a?(CodeObjects::MethodObject) && obj.scope == :class && obj.parent == object
            title = h([object.name, obj.sep, obj.name].join)
          elsif obj.title != obj.path
            title = h(obj.title)
          else
            title = h(object.relative_path(obj))
          end
        else
          title = h(obj.title)
        end
        return title unless serializer
        return title if obj.is_a?(CodeObjects::Proxy)

        link = url_for(obj, anchor, relative)
        link = link ? link_url(link, title, :title => h("#{obj.title} (#{obj.type})")) : title
        "<span class='object_link'>" + link + "</span>"
      end
      
    end
  end
end
